#include "GuiInstanceLoader_WorkflowCodegen.h"
#include "../TypeDescriptors/GuiReflectionEvents.h"
#include "../../Resources/GuiParserManager.h"

namespace vl
{
	namespace presentation
	{
		using namespace reflection::description;
		using namespace collections;

/***********************************************************************
WorkflowReferenceNamesVisitor
***********************************************************************/

		class WorkflowReferenceNamesVisitor : public Object, public GuiValueRepr::IVisitor
		{
		public:
			Ptr<GuiInstanceContext>				context;
			types::ResolvingResult&				resolvingResult;
			vint&								generatedNameCount;
			types::ErrorList&					errors;

			List<types::PropertyResolving>&		candidatePropertyTypeInfos;
			IGuiInstanceLoader::TypeInfo		resolvedTypeInfo;
			vint								selectedPropertyTypeInfo = -1;

			WorkflowReferenceNamesVisitor(Ptr<GuiInstanceContext> _context, types::ResolvingResult& _resolvingResult, List<types::PropertyResolving>& _candidatePropertyTypeInfos, vint& _generatedNameCount, types::ErrorList& _errors)
				:context(_context)
				, resolvingResult(_resolvingResult)
				, candidatePropertyTypeInfos(_candidatePropertyTypeInfos)
				, generatedNameCount(_generatedNameCount)
				, errors(_errors)
			{
			}

			void Visit(GuiTextRepr* repr)override
			{
				if (selectedPropertyTypeInfo == -1)
				{
					selectedPropertyTypeInfo = 0;
				}
				
				auto candidate = candidatePropertyTypeInfos[selectedPropertyTypeInfo];
				auto propertyInfo = candidate.propertyInfo;
				auto td = candidate.info->acceptableTypes[0];

				if (auto serializer = td->GetValueSerializer())
				{
					if (serializer->Validate(repr->text))
					{
						resolvingResult.propertyResolvings.Add(repr, candidate);
					}
					else
					{
						auto error
							= L"Precompile: Property \""
							+ propertyInfo.propertyName.ToString()
							+ L"\" of type \""
							+ propertyInfo.typeInfo.typeName.ToString()
							+ L"\" does not accept a value of text \""
							+ repr->text
							+ L"\" because it is not in a correct format of the serializable type \""
							+ td->GetTypeName()
							+ L"\".";
						errors.Add(error);
					}
				}
				else
				{
					auto error
						= L"Precompile: Property \""
						+ propertyInfo.propertyName.ToString()
						+ L"\" of type \""
						+ propertyInfo.typeInfo.typeName.ToString()
						+ L"\" does not accept a value of text \""
						+ repr->text
						+ L"\" because its type \""
						+ td->GetTypeName()
						+ L"\" is not serializable.";
					errors.Add(error);
				}
			}

			void Visit(GuiAttSetterRepr* repr)override
			{
				if (candidatePropertyTypeInfos.Count() > 0)
				{
					resolvingResult.propertyResolvings.Add(repr, candidatePropertyTypeInfos[selectedPropertyTypeInfo]);
				}

				if (repr->instanceName == GlobalStringKey::Empty)
				{
					if (resolvedTypeInfo.typeDescriptor->GetValueSerializer() == nullptr)
					{
						auto name = GlobalStringKey::Get(L"<precompile>" + itow(generatedNameCount++));
						repr->instanceName = name;
						resolvingResult.typeInfos.Add(name, resolvedTypeInfo);
					}
				}
				else if (resolvingResult.typeInfos.Keys().Contains(repr->instanceName))
				{
					errors.Add(L"Precompile: Referece name \"" + repr->instanceName.ToString() + L"\" conflict with an existing named object.");
				}
				else if (resolvedTypeInfo.typeDescriptor->GetValueSerializer())
				{
					errors.Add(L"Precompile: Reference name \"" + repr->instanceName.ToString() + L"\" cannot be added to a non-reference instance of type \"" + resolvedTypeInfo.typeName.ToString() + L"\".");
				}
				else
				{
					resolvingResult.typeInfos.Add(repr->instanceName, resolvedTypeInfo);
				}
			
				auto loader = GetInstanceLoaderManager()->GetLoader(resolvedTypeInfo.typeName);

				FOREACH_INDEXER(Ptr<GuiAttSetterRepr::SetterValue>, setter, index, repr->setters.Values())
				{
					List<types::PropertyResolving> possibleInfos;
					IGuiInstanceLoader::PropertyInfo propertyInfo(resolvedTypeInfo, repr->setters.Keys()[index]);
					auto errorPrefix = L"Precompile: Property \"" + propertyInfo.propertyName.ToString() + L"\" of type \"" + resolvedTypeInfo.typeName.ToString() + L"\"";

					{
						auto currentLoader = loader;

						while (currentLoader)
						{
							if (auto propertyTypeInfo = currentLoader->GetPropertyType(propertyInfo))
							{
								if (propertyTypeInfo->support == GuiInstancePropertyInfo::NotSupport)
								{
									errors.Add(errorPrefix + L" is not supported.");
									break;
								}
								else
								{
									types::PropertyResolving resolving;
									resolving.loader = currentLoader;
									resolving.propertyInfo = propertyInfo;
									resolving.info = propertyTypeInfo;
									possibleInfos.Add(resolving);

									if (setter->binding == GlobalStringKey::_Set)
									{
										break;
									}
								}

								if (!propertyTypeInfo->tryParent)
								{
									break;
								}
							}
							currentLoader = GetInstanceLoaderManager()->GetParentLoader(currentLoader);
						}
					}

					if (possibleInfos.Count() == 0)
					{
						errors.Add(errorPrefix + L" does not exist.");
					}
					else
					{
						if (setter->binding == GlobalStringKey::Empty)
						{
							FOREACH(Ptr<GuiValueRepr>, value, setter->values)
							{
								WorkflowReferenceNamesVisitor visitor(context, resolvingResult, possibleInfos, generatedNameCount, errors);
								value->Accept(&visitor);
							}
						}
						else  if (setter->binding == GlobalStringKey::_Set)
						{
							if (possibleInfos[0].info->support == GuiInstancePropertyInfo::SupportSet)
							{
								WorkflowReferenceNamesVisitor visitor(context, resolvingResult, possibleInfos, generatedNameCount, errors);
								auto td = possibleInfos[0].info->acceptableTypes[0];
								visitor.selectedPropertyTypeInfo = 0;
								visitor.resolvedTypeInfo.typeDescriptor = td;
								visitor.resolvedTypeInfo.typeName = GlobalStringKey::Get(td->GetTypeName());
								setter->values[0]->Accept(&visitor);
							}
							else
							{
								errors.Add(errorPrefix + L" does not support the \"-set\" binding.");
							}
						}
						else if (setter->binding != GlobalStringKey::Empty)
						{
							auto binder = GetInstanceLoaderManager()->GetInstanceBinder(setter->binding);
							if (!binder)
							{
								errors.Add(errorPrefix + L" cannot be assigned using an unexisting binding \"" + setter->binding.ToString() + L"\".");
							}

							if (setter->values.Count() == 1 && setter->values[0].Cast<GuiTextRepr>())
							{
								resolvingResult.propertyResolvings.Add(setter->values[0].Obj(), possibleInfos[0]);
							}
							else
							{
								errors.Add(L"Precompile: Binder \"" + setter->binding.ToString() + L"\" requires the text value of property \"" + propertyInfo.propertyName.ToString() + L"\".");
							}
						}
					}
				}

				FOREACH(Ptr<GuiAttSetterRepr::EventValue>, handler, repr->eventHandlers.Values())
				{
					if (handler->binding != GlobalStringKey::Empty)
					{
						auto binder = GetInstanceLoaderManager()->GetInstanceEventBinder(handler->binding);
						if (!binder)
						{
							errors.Add(L"The appropriate IGuiInstanceEventBinder of binding \"" + handler->binding.ToString() + L"\" cannot be found.");
						}
					}
				}
			}

			void Visit(GuiConstructorRepr* repr)override
			{
				bool found = false;

				if (repr == context->instance.Obj())
				{
					auto fullName = GlobalStringKey::Get(context->className);
					auto td = GetInstanceLoaderManager()->GetTypeDescriptorForType(fullName);
					if (td)
					{
						found = true;
						resolvedTypeInfo.typeName = fullName;
						resolvedTypeInfo.typeDescriptor = td;
					}
				}

				if (!found)
				{
					auto source = FindInstanceLoadingSource(context, repr);
					resolvedTypeInfo.typeName = source.typeName;
					resolvedTypeInfo.typeDescriptor = GetInstanceLoaderManager()->GetTypeDescriptorForType(source.typeName);
				}

				if (resolvedTypeInfo.typeDescriptor)
				{
					for (vint i = 0; i < candidatePropertyTypeInfos.Count(); i++)
					{
						const auto& tds = candidatePropertyTypeInfos[i].info->acceptableTypes;
						for (vint j = 0; j < tds.Count(); j++)
						{
							if (resolvedTypeInfo.typeDescriptor->CanConvertTo(tds[j]))
							{
								selectedPropertyTypeInfo = i;
								goto FINISH_MATCHING;
							}
						}
					}
				FINISH_MATCHING:

					if (selectedPropertyTypeInfo == -1 && candidatePropertyTypeInfos.Count() > 0)
					{
						auto propertyInfo = candidatePropertyTypeInfos[0].propertyInfo;
						auto error 
							= L"Precompile: Property \""
							+ propertyInfo.propertyName.ToString()
							+ L"\" of type \""
							+ propertyInfo.typeInfo.typeName.ToString()
							+ L"\" does not accept a value of type \""
							+ resolvedTypeInfo.typeName.ToKey()
							+ L"\" because it only accepts value of the following types: ";
						
						for (vint i = 0; i < candidatePropertyTypeInfos.Count(); i++)
						{
							const auto& tds = candidatePropertyTypeInfos[i].info->acceptableTypes;
							for (vint j = 0; j < tds.Count(); j++)
							{
								if (i != 0 || j != 0)
								{
									error += L", ";
								}
								error += L"\"" + tds[j]->GetTypeName() + L"\"";
							}
						}

						error += L".";
						errors.Add(error);
					}
					else
					{
						if (repr->setters.Count() == 1 && repr->setters.Keys()[0]==GlobalStringKey::Empty)
						{
							auto setter = repr->setters.Values()[0];
							if (setter->values.Count() == 1)
							{
								if (auto text = setter->values[0].Cast<GuiTextRepr>())
								{
									if (candidatePropertyTypeInfos.Count() == 0)
									{
										errors.Add(L"Precompile: Type \"" + resolvedTypeInfo.typeName.ToString() + L"\" cannot be used to create an instance.");
									}
									else
									{
										Visit(text.Obj());
									}
									return;
								}
							}
						}
						Visit((GuiAttSetterRepr*)repr);
					}
				}
				else
				{
					errors.Add(
						L"Precompile: Failed to find type \"" +
						(repr->typeNamespace == GlobalStringKey::Empty
							? repr->typeName.ToString()
							: repr->typeNamespace.ToString() + L":" + repr->typeName.ToString()
							) +
						L"\".");
				}
			}
		};

		ITypeDescriptor* Workflow_CollectReferences(Ptr<GuiInstanceContext> context, types::ResolvingResult& resolvingResult, types::ErrorList& errors)
		{
			FOREACH(Ptr<GuiInstanceParameter>, parameter, context->parameters)
			{
				auto type = GetTypeDescriptor(parameter->className.ToString());
				if (!type)
				{
					errors.Add(L"Precompile: Cannot find type \"" + parameter->className.ToString() + L"\".");
				}
				else if (resolvingResult.typeInfos.Keys().Contains(parameter->name))
				{
					errors.Add(L"Precompile: Parameter \"" + parameter->name.ToString() + L"\" conflict with an existing named object.");
				}
				else
				{
					IGuiInstanceLoader::TypeInfo typeInfo;
					typeInfo.typeDescriptor = type;
					typeInfo.typeName = GlobalStringKey::Get(type->GetTypeName());
					resolvingResult.typeInfos.Add(parameter->name, typeInfo);
				}
			}
			
			List<types::PropertyResolving> infos;
			vint generatedNameCount = 0;
			WorkflowReferenceNamesVisitor visitor(context, resolvingResult, infos, generatedNameCount, errors);
			context->instance->Accept(&visitor);
			return visitor.resolvedTypeInfo.typeDescriptor;
		}
	}
}