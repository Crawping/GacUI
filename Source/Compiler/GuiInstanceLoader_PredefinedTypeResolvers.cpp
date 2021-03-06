#include "GuiInstanceLoader.h"
#include "../Reflection/TypeDescriptors/GuiReflectionEvents.h"
#include "../Reflection/GuiInstanceCompiledWorkflow.h"
#include "../Resources/GuiParserManager.h"
#include "../Resources/GuiResourceManager.h"
#include "InstanceQuery/GuiInstanceQuery.h"
#include "GuiInstanceSharedScript.h"
#include "WorkflowCodegen/GuiInstanceLoader_WorkflowCodegen.h"

namespace vl
{
	namespace presentation
	{
		using namespace parsing;
		using namespace parsing::xml;
		using namespace workflow;
		using namespace workflow::analyzer;
		using namespace workflow::runtime;
		using namespace reflection::description;
		using namespace collections;
		using namespace stream;

		using namespace controls;

		Ptr<GuiInstanceCompiledWorkflow> Workflow_GetModule(GuiResourcePrecompileContext& context, const WString& path)
		{
			return context.targetFolder->GetValueByPath(path).Cast<GuiInstanceCompiledWorkflow>();
		}

		Ptr<GuiInstanceCompiledWorkflow> Workflow_EnsureModule(GuiResourcePrecompileContext& context, const WString& path, GuiInstanceCompiledWorkflow::AssemblyType assemblyType)
		{
			auto compiled = Workflow_GetModule(context, path);
			if (!compiled)
			{
				compiled = new GuiInstanceCompiledWorkflow;
				compiled->type = assemblyType;
				context.targetFolder->CreateValueByPath(path, L"Workflow", compiled);
			}
			else
			{
				CHECK_ERROR(compiled->type == assemblyType, L"Workflow_EnsureModule(GuiResourcePrecompiledContext&, const WString&, GuiInstanceCompiledWorkflow::AssemblyType)#Unexpected assembly type.");
			}
			return compiled;
		}

		void Workflow_AddModule(GuiResourcePrecompileContext& context, const WString& path, Ptr<WfModule> module, GuiInstanceCompiledWorkflow::AssemblyType assemblyType)
		{
			auto compiled = Workflow_EnsureModule(context, path, assemblyType);
			compiled->modules.Add(module);
		}

		void Workflow_AddModule(GuiResourcePrecompileContext& context, const WString& path, const WString& module, GuiInstanceCompiledWorkflow::AssemblyType assemblyType)
		{
			auto compiled = Workflow_EnsureModule(context, path, assemblyType);
			compiled->codes.Add(module);
		}

		void Workflow_GenerateAssembly(GuiResourcePrecompileContext& context, const WString& path, GuiResourceError::List& errors, bool keepMetadata)
		{
			auto compiled = Workflow_GetModule(context, path);
			if (!compiled)
			{
				return;
			}

			if (!compiled->assembly)
			{
				List<WString> codes;
				auto manager = Workflow_GetSharedManager();
				manager->Clear(false, true);

				auto addCode = [&codes](TextReader& reader)
				{
					vint row = 0;
					WString code;
					while (!reader.IsEnd())
					{
						auto rowHeader = itow(++row);
						while (rowHeader.Length() < 6)
						{
							rowHeader = L" " + rowHeader;
						}
						code += rowHeader + L" : " + reader.ReadLine() + L"\r\n";
					}
					codes.Add(code);
				};

				FOREACH_INDEXER(Ptr<WfModule>, module, index, compiled->modules)
				{
					{
						MemoryStream stream;
						{
							StreamWriter writer(stream);
							auto recorder = MakePtr<ParsingUpdateLocationRecorder>();
							ParsingWriter parsingWriter(writer, recorder, index);
							WfPrint(module, L"", parsingWriter);
						}
						stream.SeekFromBegin(0);
						{
							StreamReader reader(stream);
							addCode(reader);
						}
					}
					manager->AddModule(module);
				}

				FOREACH(WString, code, compiled->codes)
				{
					{
						StringReader reader(code);
						addCode(reader);
					}
					manager->AddModule(code);
				}

				if (manager->errors.Count() == 0)
				{
					manager->Rebuild(true);
				}

				if (manager->errors.Count() == 0)
				{
					compiled->assembly = GenerateAssembly(manager);
					compiled->Initialize(true);
				}
				else
				{
					errors.Add(L"Failed to compile workflow scripts in: " + path);

					using ErrorGroup = Pair<vint, LazyList<Ptr<ParsingError>>>;
					List<ErrorGroup> errorGroups;
					CopyFrom(
						errorGroups,
						From(manager->errors)
							.GroupBy([](Ptr<ParsingError> error){ return error->codeRange.codeIndex; })
							.OrderBy([](ErrorGroup g1, ErrorGroup g2){ return g1.key - g2.key; })
						);

					FOREACH(ErrorGroup, errorGroup, errorGroups)
					{
						if (errorGroup.key == -1)
						{
							FOREACH(Ptr<ParsingError>, error, errorGroup.value)
							{
								errors.Add(L"    " + error->errorMessage);
							}
						}
						else
						{
							FOREACH(Ptr<ParsingError>, error, errorGroup.value)
							{
								errors.Add(
									L"    "
									L"Row: " + itow(error->codeRange.start.row + 1) +
									L", Column: " + itow(error->codeRange.start.column + 1) +
									L", Message: " + error->errorMessage);
							}
							errors.Add(L"Workflow script for reference:");
							errors.Add(codes[errorGroup.key]);
						}
					}
					errors.Add(L"<END>");
				}

				if (keepMetadata)
				{
					compiled->metadata = Workflow_TransferSharedManager();
				}
				else
				{
					manager->Clear(false, true);
				}
			}
		}

/***********************************************************************
Shared Script Type Resolver (Script)
***********************************************************************/

#define Path_Shared				L"Workflow/Shared"
#define Path_TemporaryClass		L"Workflow/TemporaryClass"
#define Path_InstanceClass		L"Workflow/InstanceClass"

		class GuiResourceSharedScriptTypeResolver
			: public Object
			, public IGuiResourceTypeResolver
			, private IGuiResourceTypeResolver_Precompile
			, private IGuiResourceTypeResolver_IndirectLoad
		{
		public:
			WString GetType()override
			{
				return L"Script";
			}

			bool XmlSerializable()override
			{
				return true;
			}

			bool StreamSerializable()override
			{
				return false;
			}

			WString GetPreloadType()override
			{
				return L"Xml";
			}

			bool IsDelayLoad()override
			{
				return false;
			}

			vint GetMaxPassIndex()override
			{
				return Workflow_Max;
			}

			PassSupport GetPassSupport(vint passIndex)override
			{
				switch (passIndex)
				{
				case Workflow_Collect:
					return PerResource;
				case Workflow_Compile:
					return PerPass;
				default:
					return NotSupported;
				}
			}

			void PerResourcePrecompile(Ptr<GuiResourceItem> resource, GuiResourcePrecompileContext& context, GuiResourceError::List& errors)override
			{
				switch (context.passIndex)
				{
				case Workflow_Collect:
				{
					if (auto obj = resource->GetContent().Cast<GuiInstanceSharedScript>())
					{
						if (obj->language == L"Workflow")
						{
							Workflow_AddModule(context, Path_Shared, obj->code, GuiInstanceCompiledWorkflow::Shared);
						}
					}
				}
				break;
				}
			}

			void PerPassPrecompile(GuiResourcePrecompileContext& context, GuiResourceError::List& errors)override
			{
				switch (context.passIndex)
				{
				case Workflow_Compile:
					Workflow_GenerateAssembly(context, Path_Shared, errors, false);
					break;
				}
			}

			IGuiResourceTypeResolver_Precompile* Precompile()override
			{
				return this;
			}

			IGuiResourceTypeResolver_IndirectLoad* IndirectLoad()override
			{
				return this;
			}

			Ptr<DescriptableObject> Serialize(Ptr<GuiResourceItem> resource, Ptr<DescriptableObject> content)override
			{
				if (auto obj = content.Cast<GuiInstanceSharedScript>())
				{
					return obj->SaveToXml();
				}
				return nullptr;
			}

			Ptr<DescriptableObject> ResolveResource(Ptr<GuiResourceItem> resource, Ptr<GuiResourcePathResolver> resolver, GuiResourceError::List& errors)override
			{
				Ptr<XmlDocument> xml = resource->GetContent().Cast<XmlDocument>();
				if (xml)
				{
					auto schema = GuiInstanceSharedScript::LoadFromXml(resource, xml, errors);
					return schema;
				}
				return 0;
			}
		};

/***********************************************************************
Instance Type Resolver (Instance)
***********************************************************************/

		class GuiResourceInstanceTypeResolver
			: public Object
			, public IGuiResourceTypeResolver
			, private IGuiResourceTypeResolver_Precompile
			, private IGuiResourceTypeResolver_IndirectLoad
		{
		public:
			WString GetType()override
			{
				return L"Instance";
			}

			bool XmlSerializable()override
			{
				return true;
			}

			bool StreamSerializable()override
			{
				return false;
			}

			WString GetPreloadType()override
			{
				return L"Xml";
			}

			bool IsDelayLoad()override
			{
				return false;
			}

			vint GetMaxPassIndex()override
			{
				return Instance_Max;
			}

			PassSupport GetPassSupport(vint passIndex)override
			{
				switch (passIndex)
				{
				case Instance_CollectInstanceTypes:
				case Instance_CollectEventHandlers:
				case Instance_GenerateInstanceClass:
					return PerResource;
				case Instance_CompileInstanceTypes:
				case Instance_CompileEventHandlers:
				case Instance_CompileInstanceClass:
					return PerPass;
				default:
					return NotSupported;
				}
			}

#define ENSURE_ASSEMBLY_EXISTS(PATH)\
			if (auto compiled = Workflow_GetModule(context, PATH))\
			{\
				if (!compiled->assembly)\
				{\
					break;\
				}\
			}\
			else\
			{\
				break;\
			}\

#define UNLOAD_ASSEMBLY(PATH)\
			if (auto compiled = Workflow_GetModule(context, PATH))\
			{\
				compiled->context = nullptr;\
			}\

#define DELETE_ASSEMBLY(PATH)\
			if (auto compiled = Workflow_GetModule(context, PATH))\
			{\
				compiled->context = nullptr;\
				compiled->assembly = nullptr;\
			}\

			void PerResourcePrecompile(Ptr<GuiResourceItem> resource, GuiResourcePrecompileContext& context, GuiResourceError::List& errors)override
			{
				switch (context.passIndex)
				{
				case Instance_CollectEventHandlers:
					ENSURE_ASSEMBLY_EXISTS(Path_TemporaryClass)
				case Instance_CollectInstanceTypes:
					{
						if (auto obj = resource->GetContent().Cast<GuiInstanceContext>())
						{
							obj->ApplyStyles(resource, context.resolver, errors);

							types::ResolvingResult resolvingResult;
							resolvingResult.resource = resource;
							resolvingResult.context = obj;
							if (auto module = Workflow_GenerateInstanceClass(resolvingResult, errors, context.passIndex))
							{
								Workflow_AddModule(context, Path_TemporaryClass, module, GuiInstanceCompiledWorkflow::TemporaryClass);
							}

							if (context.passIndex == Instance_CollectInstanceTypes)
							{
								auto record = context.targetFolder->GetValueByPath(L"ClassNameRecord").Cast<GuiResourceClassNameRecord>();
								if (!record)
								{
									record = MakePtr<GuiResourceClassNameRecord>();
									context.targetFolder->CreateValueByPath(L"ClassNameRecord", L"ClassNameRecord", record);
								}
								record->classNames.Add(obj->className);
							}
						}
					}
					break;
				case Instance_GenerateInstanceClass:
					{
						ENSURE_ASSEMBLY_EXISTS(Path_TemporaryClass)
						if (auto obj = resource->GetContent().Cast<GuiInstanceContext>())
						{
							vint previousErrorCount = errors.Count();
							if (obj->className == L"")
							{
								errors.Add(GuiResourceError(resource, obj->classPosition,
									L"Precompile: Instance  \"" +
									(obj->instance->typeNamespace == GlobalStringKey::Empty
										? obj->instance->typeName.ToString()
										: obj->instance->typeNamespace.ToString() + L":" + obj->instance->typeName.ToString()
										) +
									L"\" should have the class name specified in the ref.Class attribute."));
							}

							types::ResolvingResult resolvingResult;
							resolvingResult.resource = resource;
							resolvingResult.context = obj;
							resolvingResult.rootTypeDescriptor = Workflow_CollectReferences(resolvingResult, errors);

							if (errors.Count() == previousErrorCount)
							{
								if (auto ctorModule = Workflow_PrecompileInstanceContext(resolvingResult, errors))
								{
									if (auto instanceModule = Workflow_GenerateInstanceClass(resolvingResult, errors, context.passIndex))
									{
										Workflow_AddModule(context, Path_InstanceClass, ctorModule, GuiInstanceCompiledWorkflow::InstanceClass);
										Workflow_AddModule(context, Path_InstanceClass, instanceModule, GuiInstanceCompiledWorkflow::InstanceClass);
									}
								}
							}
						}
					}
					break;
				}
			}

			void PerPassPrecompile(GuiResourcePrecompileContext& context, GuiResourceError::List& errors)override
			{
				WString path;
				switch (context.passIndex)
				{
				case Instance_CompileInstanceTypes:
					DELETE_ASSEMBLY(Path_Shared)
					path = Path_TemporaryClass;
					break;
				case Instance_CompileEventHandlers:
					DELETE_ASSEMBLY(Path_TemporaryClass)
					path = Path_TemporaryClass;
					break;
				case Instance_CompileInstanceClass:
					UNLOAD_ASSEMBLY(Path_TemporaryClass)
					path = Path_InstanceClass;
					break;
				default:
					return;
				}

				auto sharedCompiled = Workflow_GetModule(context, Path_Shared);
				auto compiled = Workflow_GetModule(context, path);
				if (sharedCompiled && compiled)
				{
					CopyFrom(compiled->codes, sharedCompiled->codes);
				}

				switch (context.passIndex)
				{
				case Instance_CompileInstanceTypes:
					Workflow_GenerateAssembly(context, path, errors, false);
					compiled->modules.Clear();
					break;
				case Instance_CompileEventHandlers:
					Workflow_GenerateAssembly(context, path, errors, false);
					break;
				case Instance_CompileInstanceClass:
					Workflow_GenerateAssembly(context, path, errors, true);
					break;
				default:;
				}
				GetInstanceLoaderManager()->ClearReflectionCache();
			}

#undef DELETE_ASSEMBLY
#undef UNLOAD_ASSEMBLY
#undef ENSURE_ASSEMBLY_EXISTS

			IGuiResourceTypeResolver_Precompile* Precompile()override
			{
				return this;
			}

			IGuiResourceTypeResolver_IndirectLoad* IndirectLoad()override
			{
				return this;
			}

			Ptr<DescriptableObject> Serialize(Ptr<GuiResourceItem> resource, Ptr<DescriptableObject> content)override
			{
				if (auto obj = content.Cast<GuiInstanceContext>())
				{
					return obj->SaveToXml();
				}
				return 0;
			}

			Ptr<DescriptableObject> ResolveResource(Ptr<GuiResourceItem> resource, Ptr<GuiResourcePathResolver> resolver, GuiResourceError::List& errors)override
			{
				Ptr<XmlDocument> xml = resource->GetContent().Cast<XmlDocument>();
				if (xml)
				{
					Ptr<GuiInstanceContext> context = GuiInstanceContext::LoadFromXml(resource, xml, errors);
					return context;
				}
				return 0;
			}
		};

#undef Path_Shared
#undef Path_TemporaryClass
#undef Path_InstanceClass

/***********************************************************************
Instance Style Type Resolver (InstanceStyle)
***********************************************************************/

		class GuiResourceInstanceStyleResolver
			: public Object
			, public IGuiResourceTypeResolver
			, private IGuiResourceTypeResolver_IndirectLoad
		{
		public:
			WString GetType()override
			{
				return L"InstanceStyle";
			}

			bool XmlSerializable()override
			{
				return true;
			}

			bool StreamSerializable()override
			{
				return false;
			}

			WString GetPreloadType()override
			{
				return L"Xml";
			}

			bool IsDelayLoad()override
			{
				return false;
			}

			IGuiResourceTypeResolver_IndirectLoad* IndirectLoad()override
			{
				return this;
			}

			Ptr<DescriptableObject> Serialize(Ptr<GuiResourceItem> resource, Ptr<DescriptableObject> content)override
			{
				if (auto obj = content.Cast<GuiInstanceStyleContext>())
				{
					return obj->SaveToXml();
				}
				return 0;
			}

			Ptr<DescriptableObject> ResolveResource(Ptr<GuiResourceItem> resource, Ptr<GuiResourcePathResolver> resolver, GuiResourceError::List& errors)override
			{
				Ptr<XmlDocument> xml = resource->GetContent().Cast<XmlDocument>();
				if (xml)
				{
					auto context = GuiInstanceStyleContext::LoadFromXml(resource, xml, errors);
					return context;
				}
				return 0;
			}
		};

/***********************************************************************
Plugin
***********************************************************************/

		class GuiCompilerTypeResolversPlugin : public Object, public IGuiPlugin
		{
		public:
			GuiCompilerTypeResolversPlugin()
			{
			}

			void Load()override
			{
			}

			void AfterLoad()override
			{
				IGuiResourceResolverManager* manager = GetResourceResolverManager();
				manager->SetTypeResolver(new GuiResourceInstanceTypeResolver);
				manager->SetTypeResolver(new GuiResourceInstanceStyleResolver);
				manager->SetTypeResolver(new GuiResourceSharedScriptTypeResolver);
			}

			void Unload()override
			{
			}
		};
		GUI_REGISTER_PLUGIN(GuiCompilerTypeResolversPlugin)
	}
}
