#include "GuiReflectionEvents.h"

namespace vl
{
	namespace reflection
	{
		namespace description
		{
			using namespace collections;
			using namespace parsing;
			using namespace parsing::tabling;
			using namespace parsing::xml;
			using namespace stream;
			using namespace presentation;
			using namespace presentation::elements;
			using namespace presentation::compositions;
			using namespace presentation::controls;

#ifndef VCZH_DEBUG_NO_REFLECTION

			GUIREFLECTIONBASIC_TYPELIST(IMPL_VL_TYPE_INFO)

/***********************************************************************
Type Declaration
***********************************************************************/

#define _ ,
			BEGIN_STRUCT_MEMBER(Color)
				valueType = new SerializableValueType<Color>();
				serializableType = new SerializableType<Color>();
				STRUCT_MEMBER(r)
				STRUCT_MEMBER(g)
				STRUCT_MEMBER(b)
				STRUCT_MEMBER(a)
				STRUCT_MEMBER(value)
			END_STRUCT_MEMBER(Color)

			BEGIN_ENUM_ITEM(Alignment)
				ENUM_CLASS_ITEM(Left)
				ENUM_CLASS_ITEM(Top)
				ENUM_CLASS_ITEM(Center)
				ENUM_CLASS_ITEM(Right)
				ENUM_CLASS_ITEM(Bottom)
			END_ENUM_ITEM(Alignment)

			BEGIN_ENUM_ITEM(AxisDirection)
				ENUM_CLASS_ITEM(LeftDown)
				ENUM_CLASS_ITEM(RightDown)
				ENUM_CLASS_ITEM(LeftUp)
				ENUM_CLASS_ITEM(RightUp)
				ENUM_CLASS_ITEM(DownLeft)
				ENUM_CLASS_ITEM(DownRight)
				ENUM_CLASS_ITEM(UpLeft)
				ENUM_CLASS_ITEM(UpRight)
			END_ENUM_ITEM(AxisDirection)

			BEGIN_STRUCT_MEMBER(TextPos)
				STRUCT_MEMBER(row)
				STRUCT_MEMBER(column)
			END_STRUCT_MEMBER(TextPos)

			BEGIN_STRUCT_MEMBER(GridPos)
				STRUCT_MEMBER(row)
				STRUCT_MEMBER(column)
			END_STRUCT_MEMBER(GridPos)

			BEGIN_STRUCT_MEMBER(Point)
				STRUCT_MEMBER(x)
				STRUCT_MEMBER(y)
			END_STRUCT_MEMBER(Point)

			BEGIN_STRUCT_MEMBER(Size)
				STRUCT_MEMBER(x)
				STRUCT_MEMBER(y)
			END_STRUCT_MEMBER(Size)

			BEGIN_STRUCT_MEMBER(Rect)
				STRUCT_MEMBER(x1)
				STRUCT_MEMBER(y1)
				STRUCT_MEMBER(x2)
				STRUCT_MEMBER(y2)
			END_STRUCT_MEMBER(Rect)

			BEGIN_STRUCT_MEMBER(Margin)
				STRUCT_MEMBER(left)
				STRUCT_MEMBER(top)
				STRUCT_MEMBER(right)
				STRUCT_MEMBER(bottom)
			END_STRUCT_MEMBER(Margin)

			BEGIN_STRUCT_MEMBER(FontProperties)
				STRUCT_MEMBER(fontFamily)
				STRUCT_MEMBER(size)
				STRUCT_MEMBER(bold)
				STRUCT_MEMBER(italic)
				STRUCT_MEMBER(underline)
				STRUCT_MEMBER(strikeline)
				STRUCT_MEMBER(antialias)
				STRUCT_MEMBER(verticalAntialias)
			END_STRUCT_MEMBER(FontProperties)

			BEGIN_STRUCT_MEMBER_FLAG(GlobalStringKey, TypeDescriptorFlags::Primitive)
				valueType = new SerializableValueType<GlobalStringKey>();
				serializableType = new SerializableType<GlobalStringKey>();
			END_STRUCT_MEMBER(GlobalStringKey)

			BEGIN_INTERFACE_MEMBER_NOPROXY(INativeImageFrame)
				CLASS_MEMBER_METHOD(GetImage, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetSize, NO_PARAMETER)
			END_INTERFACE_MEMBER(INativeImageFrame)

			BEGIN_INTERFACE_MEMBER_NOPROXY(INativeImage)
				CLASS_MEMBER_METHOD(GetFormat, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetFrameCount, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetFrame, {L"index"})
				CLASS_MEMBER_EXTERNALCTOR(Ptr<INativeImage>(const WString&), {L"filePath"}, vl::reflection::description::INativeImage_Constructor)
			END_INTERFACE_MEMBER(INativeImage)

			BEGIN_ENUM_ITEM(INativeImage::FormatType)
				ENUM_ITEM_NAMESPACE(INativeImage)
				ENUM_NAMESPACE_ITEM(Bmp)
				ENUM_NAMESPACE_ITEM(Gif)
				ENUM_NAMESPACE_ITEM(Icon)
				ENUM_NAMESPACE_ITEM(Jpeg)
				ENUM_NAMESPACE_ITEM(Png)
				ENUM_NAMESPACE_ITEM(Tiff)
				ENUM_NAMESPACE_ITEM(Wmp)
				ENUM_NAMESPACE_ITEM(Unknown)
			END_ENUM_ITEM(INativeImage::FormatType)

			BEGIN_INTERFACE_MEMBER_NOPROXY(INativeCursor)
				CLASS_MEMBER_METHOD(IsSystemCursor, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetSystemCursorType, NO_PARAMETER)
				CLASS_MEMBER_EXTERNALCTOR(INativeCursor*(), NO_PARAMETER, vl::reflection::description::INativeCursor_Constructor1)
				CLASS_MEMBER_EXTERNALCTOR(INativeCursor*(INativeCursor::SystemCursorType), NO_PARAMETER, vl::reflection::description::INativeCursor_Constructor2)
			END_INTERFACE_MEMBER(INativeCursor)

			BEGIN_ENUM_ITEM(INativeCursor::SystemCursorType)
				ENUM_ITEM_NAMESPACE(INativeCursor)
				ENUM_NAMESPACE_ITEM(SmallWaiting)
				ENUM_NAMESPACE_ITEM(LargeWaiting)
				ENUM_NAMESPACE_ITEM(Arrow)
				ENUM_NAMESPACE_ITEM(Cross)
				ENUM_NAMESPACE_ITEM(Hand)
				ENUM_NAMESPACE_ITEM(Help)
				ENUM_NAMESPACE_ITEM(IBeam)
				ENUM_NAMESPACE_ITEM(SizeAll)
				ENUM_NAMESPACE_ITEM(SizeNESW)
				ENUM_NAMESPACE_ITEM(SizeNS)
				ENUM_NAMESPACE_ITEM(SizeNWSE)
				ENUM_NAMESPACE_ITEM(SizeWE)
			END_ENUM_ITEM(INativeCursor::SystemCursorType)

			BEGIN_INTERFACE_MEMBER_NOPROXY(INativeWindow)
				CLASS_MEMBER_PROPERTY_FAST(Bounds)
				CLASS_MEMBER_PROPERTY_FAST(ClientSize)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ClientBoundsInScreen)
				CLASS_MEMBER_PROPERTY_FAST(Title)
				CLASS_MEMBER_PROPERTY_FAST(WindowCursor)
				CLASS_MEMBER_PROPERTY_FAST(CaretPoint)
				CLASS_MEMBER_PROPERTY_FAST(Parent)
				CLASS_MEMBER_PROPERTY_FAST(AlwaysPassFocusToParent)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(SizeState)
				CLASS_MEMBER_PROPERTY_FAST(MinimizedBox)
				CLASS_MEMBER_PROPERTY_FAST(MaximizedBox)
				CLASS_MEMBER_PROPERTY_FAST(Border)
				CLASS_MEMBER_PROPERTY_FAST(SizeBox)
				CLASS_MEMBER_PROPERTY_FAST(IconVisible)
				CLASS_MEMBER_PROPERTY_FAST(TitleBar)
				CLASS_MEMBER_PROPERTY_FAST(TopMost)

				CLASS_MEMBER_METHOD(EnableCustomFrameMode, NO_PARAMETER)
				CLASS_MEMBER_METHOD(DisableCustomFrameMode, NO_PARAMETER)
				CLASS_MEMBER_METHOD(IsCustomFrameModeEnabled, NO_PARAMETER)
				CLASS_MEMBER_METHOD(Show, NO_PARAMETER)
				CLASS_MEMBER_METHOD(ShowDeactivated, NO_PARAMETER)
				CLASS_MEMBER_METHOD(ShowRestored, NO_PARAMETER)
				CLASS_MEMBER_METHOD(ShowMaximized, NO_PARAMETER)
				CLASS_MEMBER_METHOD(ShowMinimized, NO_PARAMETER)
				CLASS_MEMBER_METHOD(Hide, NO_PARAMETER)
				CLASS_MEMBER_METHOD(IsVisible, NO_PARAMETER)
				CLASS_MEMBER_METHOD(Enable, NO_PARAMETER)
				CLASS_MEMBER_METHOD(Disable, NO_PARAMETER)
				CLASS_MEMBER_METHOD(IsEnabled, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SetFocus, NO_PARAMETER)
				CLASS_MEMBER_METHOD(IsFocused, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SetActivate, NO_PARAMETER)
				CLASS_MEMBER_METHOD(IsActivated, NO_PARAMETER)
				CLASS_MEMBER_METHOD(ShowInTaskBar, NO_PARAMETER)
				CLASS_MEMBER_METHOD(HideInTaskBar, NO_PARAMETER)
				CLASS_MEMBER_METHOD(IsAppearedInTaskBar, NO_PARAMETER)
				CLASS_MEMBER_METHOD(EnableActivate, NO_PARAMETER)
				CLASS_MEMBER_METHOD(DisableActivate, NO_PARAMETER)
				CLASS_MEMBER_METHOD(IsEnabledActivate, NO_PARAMETER)
				CLASS_MEMBER_METHOD(RequireCapture, NO_PARAMETER)
				CLASS_MEMBER_METHOD(ReleaseCapture, NO_PARAMETER)
				CLASS_MEMBER_METHOD(IsCapturing, NO_PARAMETER)
				CLASS_MEMBER_METHOD(RedrawContent, NO_PARAMETER)
			END_INTERFACE_MEMBER(INativeWindow)

			BEGIN_ENUM_ITEM(INativeWindow::WindowSizeState)
				ENUM_ITEM_NAMESPACE(INativeWindow)
				ENUM_NAMESPACE_ITEM(Minimized)
				ENUM_NAMESPACE_ITEM(Restored)
				ENUM_NAMESPACE_ITEM(Maximized)
			END_ENUM_ITEM(INativeWindow::WindowSizeState)

			BEGIN_INTERFACE_MEMBER_NOPROXY(INativeDelay)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Status)

				CLASS_MEMBER_METHOD(Delay, {L"milliseconds"})
				CLASS_MEMBER_METHOD(Cancel, NO_PARAMETER)
			END_INTERFACE_MEMBER(INativeDelay)

			BEGIN_ENUM_ITEM(INativeDelay::ExecuteStatus)
				ENUM_ITEM_NAMESPACE(INativeDelay)
				ENUM_NAMESPACE_ITEM(Pending)
				ENUM_NAMESPACE_ITEM(Executing)
				ENUM_NAMESPACE_ITEM(Executed)
				ENUM_NAMESPACE_ITEM(Canceled)
			END_ENUM_ITEM(INativeDelay::ExecuteStatus)

			BEGIN_INTERFACE_MEMBER_NOPROXY(INativeScreen)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Bounds);
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ClientBounds);
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Name);

				CLASS_MEMBER_METHOD(IsPrimary, NO_PARAMETER)
			END_INTERFACE_MEMBER(INativeScreen)

			BEGIN_INTERFACE_MEMBER_NOPROXY(INativeImageService)
				CLASS_MEMBER_METHOD(CreateImageFromFile, {L"path"})
			END_INTERFACE_MEMBER(INativeImageService)

			BEGIN_INTERFACE_MEMBER_NOPROXY(INativeResourceService)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(DefaultSystemCursor)
				CLASS_MEMBER_PROPERTY_FAST(DefaultFont)

				CLASS_MEMBER_METHOD(GetSystemCursor, {L"type"})
			END_INTERFACE_MEMBER(INativeResourceService)

			BEGIN_INTERFACE_MEMBER_NOPROXY(INativeAsyncService)
				CLASS_MEMBER_METHOD(IsInMainThread, {L"type"})
				CLASS_MEMBER_METHOD(InvokeAsync, {L"proc"})
				CLASS_MEMBER_METHOD(InvokeInMainThread, {L"proc"})
				CLASS_MEMBER_METHOD(InvokeInMainThreadAndWait, {L"proc" _ L"milliseconds"})
				CLASS_MEMBER_METHOD(DelayExecute, {L"proc" _ L"milliseconds"})
				CLASS_MEMBER_METHOD(DelayExecuteInMainThread, {L"proc" _ L"milliseconds"})
			END_INTERFACE_MEMBER(INativeAsyncService)

			BEGIN_INTERFACE_MEMBER_NOPROXY(INativeClipboardService)
				CLASS_MEMBER_PROPERTY_FAST(Text)

				CLASS_MEMBER_METHOD(ContainsText, NO_PARAMETER)
			END_INTERFACE_MEMBER(INativeClipboardService)

			BEGIN_INTERFACE_MEMBER_NOPROXY(INativeScreenService)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ScreenCount)

				CLASS_MEMBER_METHOD_OVERLOAD(GetScreen, {L"index"}, INativeScreen*(INativeScreenService::*)(vint))
				CLASS_MEMBER_METHOD_OVERLOAD(GetScreen, {L"window"}, INativeScreen*(INativeScreenService::*)(INativeWindow*))
			END_INTERFACE_MEMBER(INativeScreenService)

			BEGIN_INTERFACE_MEMBER_NOPROXY(INativeInputService)
				CLASS_MEMBER_METHOD(IsKeyPressing, { L"code" })
				CLASS_MEMBER_METHOD(IsKeyToggled, { L"code" })
				CLASS_MEMBER_METHOD(GetKeyName, { L"code" })
				CLASS_MEMBER_METHOD(GetKey, { L"name" })
			END_INTERFACE_MEMBER(INativeInputService)

			BEGIN_ENUM_ITEM(INativeDialogService::MessageBoxButtonsInput)
				ENUM_ITEM_NAMESPACE(INativeDialogService)
				ENUM_NAMESPACE_ITEM(DisplayOK)
				ENUM_NAMESPACE_ITEM(DisplayOKCancel)
				ENUM_NAMESPACE_ITEM(DisplayYesNo)
				ENUM_NAMESPACE_ITEM(DisplayYesNoCancel)
				ENUM_NAMESPACE_ITEM(DisplayRetryCancel)
				ENUM_NAMESPACE_ITEM(DisplayAbortRetryIgnore)
				ENUM_NAMESPACE_ITEM(DisplayCancelTryAgainContinue)
			END_ENUM_ITEM(INativeDialogService::MessageBoxButtonsInput)

			BEGIN_ENUM_ITEM(INativeDialogService::MessageBoxButtonsOutput)
				ENUM_ITEM_NAMESPACE(INativeDialogService)
				ENUM_NAMESPACE_ITEM(SelectOK)
				ENUM_NAMESPACE_ITEM(SelectCancel)
				ENUM_NAMESPACE_ITEM(SelectYes)
				ENUM_NAMESPACE_ITEM(SelectNo)
				ENUM_NAMESPACE_ITEM(SelectRetry)
				ENUM_NAMESPACE_ITEM(SelectAbort)
				ENUM_NAMESPACE_ITEM(SelectIgnore)
				ENUM_NAMESPACE_ITEM(SelectTryAgain)
				ENUM_NAMESPACE_ITEM(SelectContinue)
			END_ENUM_ITEM(INativeDialogService::MessageBoxButtonsOutput)

			BEGIN_ENUM_ITEM(INativeDialogService::MessageBoxDefaultButton)
				ENUM_ITEM_NAMESPACE(INativeDialogService)
				ENUM_NAMESPACE_ITEM(DefaultFirst)
				ENUM_NAMESPACE_ITEM(DefaultSecond)
				ENUM_NAMESPACE_ITEM(DefaultThird)
			END_ENUM_ITEM(INativeDialogService::MessageBoxDefaultButton)

			BEGIN_ENUM_ITEM(INativeDialogService::MessageBoxIcons)
				ENUM_ITEM_NAMESPACE(INativeDialogService)
				ENUM_NAMESPACE_ITEM(IconNone)
				ENUM_NAMESPACE_ITEM(IconError)
				ENUM_NAMESPACE_ITEM(IconQuestion)
				ENUM_NAMESPACE_ITEM(IconWarning)
				ENUM_NAMESPACE_ITEM(IconInformation)
			END_ENUM_ITEM(INativeDialogService::MessageBoxIcons)

			BEGIN_ENUM_ITEM(INativeDialogService::MessageBoxModalOptions)
				ENUM_ITEM_NAMESPACE(INativeDialogService)
				ENUM_NAMESPACE_ITEM(ModalWindow)
				ENUM_NAMESPACE_ITEM(ModalTask)
				ENUM_NAMESPACE_ITEM(ModalSystem)
			END_ENUM_ITEM(INativeDialogService::MessageBoxModalOptions)

			BEGIN_ENUM_ITEM(INativeDialogService::ColorDialogCustomColorOptions)
				ENUM_ITEM_NAMESPACE(INativeDialogService)
				ENUM_NAMESPACE_ITEM(CustomColorDisabled)
				ENUM_NAMESPACE_ITEM(CustomColorEnabled)
				ENUM_NAMESPACE_ITEM(CustomColorOpened)
			END_ENUM_ITEM(INativeDialogService::ColorDialogCustomColorOptions)

			BEGIN_ENUM_ITEM(INativeDialogService::FileDialogTypes)
				ENUM_ITEM_NAMESPACE(INativeDialogService)
				ENUM_NAMESPACE_ITEM(FileDialogOpen)
				ENUM_NAMESPACE_ITEM(FileDialogOpenPreview)
				ENUM_NAMESPACE_ITEM(FileDialogSave)
				ENUM_NAMESPACE_ITEM(FileDialogSavePreview)
			END_ENUM_ITEM(INativeDialogService::FileDialogTypes)

			BEGIN_ENUM_ITEM_MERGABLE(INativeDialogService::FileDialogOptions)
				ENUM_ITEM_NAMESPACE(INativeDialogService)
				ENUM_NAMESPACE_ITEM(FileDialogAllowMultipleSelection)
				ENUM_NAMESPACE_ITEM(FileDialogFileMustExist)
				ENUM_NAMESPACE_ITEM(FileDialogShowReadOnlyCheckBox)
				ENUM_NAMESPACE_ITEM(FileDialogDereferenceLinks)
				ENUM_NAMESPACE_ITEM(FileDialogShowNetworkButton)
				ENUM_NAMESPACE_ITEM(FileDialogPromptCreateFile)
				ENUM_NAMESPACE_ITEM(FileDialogPromptOverwriteFile)
				ENUM_NAMESPACE_ITEM(FileDialogDirectoryMustExist)
				ENUM_NAMESPACE_ITEM(FileDialogAddToRecent)
			END_ENUM_ITEM(INativeDialogService::FileDialogOptions)

			BEGIN_INTERFACE_MEMBER_NOPROXY(INativeController)
				CLASS_MEMBER_STATIC_EXTERNALMETHOD(GetCurrentController, NO_PARAMETER, INativeController*(*)(), vl::reflection::description::GetCurrentController)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(OSVersion)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ExecutablePath)

				CLASS_MEMBER_METHOD(ResourceService, NO_PARAMETER)
				CLASS_MEMBER_METHOD(AsyncService, NO_PARAMETER)
				CLASS_MEMBER_METHOD(ClipboardService, NO_PARAMETER)
				CLASS_MEMBER_METHOD(ImageService, NO_PARAMETER)
				CLASS_MEMBER_METHOD(ScreenService, NO_PARAMETER)
				CLASS_MEMBER_METHOD(InputService, NO_PARAMETER)
			END_INTERFACE_MEMBER(INativeController)

			BEGIN_CLASS_MEMBER(GuiImageData)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<GuiImageData>(), NO_PARAMETER)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<GuiImageData>(Ptr<INativeImage>, vint), {L"image" _ L"frameIndex"})

				CLASS_MEMBER_METHOD(GetImage, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetFrameIndex, NO_PARAMETER)

				CLASS_MEMBER_PROPERTY_READONLY(Image, GetImage)
				CLASS_MEMBER_PROPERTY_READONLY(FrameIndex, GetFrameIndex)
			END_CLASS_MEMBER(GuiImageData)

			BEGIN_CLASS_MEMBER(GuiTextData)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<GuiTextData>(), NO_PARAMETER)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<GuiTextData>(const WString&), {L"text"})

				CLASS_MEMBER_PROPERTY_READONLY_FAST(Text)
			END_CLASS_MEMBER(GuiTextData)
				
			BEGIN_STRUCT_MEMBER(DocumentFontSize)
				valueType = new SerializableValueType<DocumentFontSize>();
				serializableType = new SerializableType<DocumentFontSize>();
				STRUCT_MEMBER(size)
				STRUCT_MEMBER(relative)
			END_STRUCT_MEMBER(DocumentFontSize)

			BEGIN_CLASS_MEMBER(DocumentStyleProperties)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<DocumentStyleProperties>(), NO_PARAMETER)

				CLASS_MEMBER_FIELD(face)
				CLASS_MEMBER_FIELD(size)
				CLASS_MEMBER_FIELD(color)
				CLASS_MEMBER_FIELD(backgroundColor)
				CLASS_MEMBER_FIELD(bold)
				CLASS_MEMBER_FIELD(italic)
				CLASS_MEMBER_FIELD(underline)
				CLASS_MEMBER_FIELD(strikeline)
				CLASS_MEMBER_FIELD(antialias)
				CLASS_MEMBER_FIELD(verticalAntialias)
			END_CLASS_MEMBER(DocumentStyleProperties)

			BEGIN_CLASS_MEMBER(DocumentRun)
			END_CLASS_MEMBER(DocumentRun)

			BEGIN_CLASS_MEMBER(DocumentContainerRun)
				CLASS_MEMBER_BASE(DocumentRun)

				CLASS_MEMBER_FIELD(runs)
			END_CLASS_MEMBER(DocumentContainerRun)

			BEGIN_CLASS_MEMBER(DocumentContentRun)
				CLASS_MEMBER_BASE(DocumentRun)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(RepresentationText)
			END_CLASS_MEMBER(DocumentContentRun)

			BEGIN_CLASS_MEMBER(DocumentTextRun)
				CLASS_MEMBER_BASE(DocumentContentRun)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<DocumentTextRun>(), NO_PARAMETER)
				
				CLASS_MEMBER_FIELD(text)
			END_CLASS_MEMBER(DocumentTextRun)

			BEGIN_CLASS_MEMBER(DocumentInlineObjectRun)
				CLASS_MEMBER_BASE(DocumentContentRun)
				
				CLASS_MEMBER_FIELD(size)
				CLASS_MEMBER_FIELD(baseline)
			END_CLASS_MEMBER(DocumentInlineObjectRun)

			BEGIN_CLASS_MEMBER(DocumentImageRun)
				CLASS_MEMBER_BASE(DocumentInlineObjectRun)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<DocumentImageRun>(), NO_PARAMETER)
				
				CLASS_MEMBER_FIELD(image)
				CLASS_MEMBER_FIELD(frameIndex)
				CLASS_MEMBER_FIELD(source)
			END_CLASS_MEMBER(DocumentImageRun)

			BEGIN_CLASS_MEMBER(DocumentEmbeddedObjectRun)
				CLASS_MEMBER_BASE(DocumentInlineObjectRun)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<DocumentEmbeddedObjectRun>(), NO_PARAMETER)
				
				CLASS_MEMBER_FIELD(name)
			END_CLASS_MEMBER(DocumentEmbeddedObjectRun)

			BEGIN_CLASS_MEMBER(DocumentStylePropertiesRun)
				CLASS_MEMBER_BASE(DocumentContainerRun)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<DocumentStylePropertiesRun>(), NO_PARAMETER)
				
				CLASS_MEMBER_FIELD(style)
			END_CLASS_MEMBER(DocumentStylePropertiesRun)

			BEGIN_CLASS_MEMBER(DocumentStyleApplicationRun)
				CLASS_MEMBER_BASE(DocumentContainerRun)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<DocumentStyleApplicationRun>(), NO_PARAMETER)
				
				CLASS_MEMBER_FIELD(styleName)
			END_CLASS_MEMBER(DocumentStyleApplicationRun)

			BEGIN_CLASS_MEMBER(DocumentHyperlinkRun)
				CLASS_MEMBER_BASE(DocumentStyleApplicationRun)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<DocumentHyperlinkRun>(), NO_PARAMETER)
				
				CLASS_MEMBER_FIELD(normalStyleName)
				CLASS_MEMBER_FIELD(activeStyleName)
				CLASS_MEMBER_FIELD(reference)
			END_CLASS_MEMBER(DocumentHyperlinkRun)

			BEGIN_CLASS_MEMBER(DocumentParagraphRun)
				CLASS_MEMBER_BASE(DocumentContainerRun)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<DocumentParagraphRun>(), NO_PARAMETER)

				CLASS_MEMBER_FIELD(alignment)

				CLASS_MEMBER_METHOD_OVERLOAD(GetText, {L"skipNonTextContent"}, WString(DocumentParagraphRun::*)(bool))
			END_CLASS_MEMBER(DocumentParagraphRun)

			BEGIN_CLASS_MEMBER(DocumentStyle)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<DocumentStyle>(), NO_PARAMETER)
				
				CLASS_MEMBER_FIELD(parentStyleName)
				CLASS_MEMBER_FIELD(styles)
				CLASS_MEMBER_FIELD(resolvedStyles)
			END_CLASS_MEMBER(DocumentStyle)

			BEGIN_CLASS_MEMBER(DocumentModel)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<DocumentModel>(), NO_PARAMETER)
				
				CLASS_MEMBER_FIELD(paragraphs)
				CLASS_MEMBER_FIELD(styles)

				CLASS_MEMBER_METHOD_OVERLOAD(GetText, {L"skipNonTextContent"}, WString(DocumentModel::*)(bool))
				CLASS_MEMBER_STATIC_METHOD(LoadFromXml, {L"resource" _ L"xml" _ L"workingDirectory" _ L"errors"})
				CLASS_MEMBER_METHOD_OVERLOAD(SaveToXml, NO_PARAMETER, Ptr<XmlDocument>(DocumentModel::*)())
			END_CLASS_MEMBER(DocumentModel)

			BEGIN_CLASS_MEMBER(GuiResourceNodeBase)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Parent)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Name)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ResourcePath)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(FileContentPath)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(FileAbsolutePath)
				CLASS_MEMBER_METHOD(SetFileContentPath, { L"content" _ L"absolute" })
			END_CLASS_MEMBER(GuiResourceNodeBase)

			BEGIN_STRUCT_MEMBER(GuiResourceError)
				STRUCT_MEMBER(resourcePath)
				STRUCT_MEMBER(filePath)
				STRUCT_MEMBER(position)
				STRUCT_MEMBER(message)
			END_STRUCT_MEMBER(GuiResourceError)

			BEGIN_CLASS_MEMBER(GuiResourceItem)
				CLASS_MEMBER_BASE(GuiResourceNodeBase)
				
				CLASS_MEMBER_METHOD(GetContent, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SetContent, {L"typeName" _ L"value"})

				CLASS_MEMBER_METHOD(AsImage, NO_PARAMETER)
				CLASS_MEMBER_METHOD(AsXml, NO_PARAMETER)
				CLASS_MEMBER_METHOD(AsString, NO_PARAMETER)
				CLASS_MEMBER_METHOD(AsDocument, NO_PARAMETER)
			END_CLASS_MEMBER(GuiResourceItem)

			BEGIN_CLASS_MEMBER(GuiResourceFolder)
				CLASS_MEMBER_BASE(GuiResourceNodeBase)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(Items)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Folders)

				CLASS_MEMBER_METHOD(GetItem, { L"name" })
				CLASS_MEMBER_METHOD(AddItem, { L"name" _ L"item" })
				CLASS_MEMBER_METHOD(RemoveItem, { L"name" })
				CLASS_MEMBER_METHOD(ClearItems, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetFolder, { L"name" })
				CLASS_MEMBER_METHOD(AddFolder, { L"name" _ L"folder" })
				CLASS_MEMBER_METHOD(RemoveFolder, { L"name" })
				CLASS_MEMBER_METHOD(ClearFolders, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetValueByPath, { L"path" })
				CLASS_MEMBER_METHOD(GetFolderByPath, { L"path" })
				CLASS_MEMBER_METHOD(CreateValueByPath, { L"path" _ L"typeName" _ L"value" })
			END_CLASS_MEMBER(GuiResourceFolder)

			BEGIN_CLASS_MEMBER(GuiResource)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<GuiResource>(), NO_PARAMETER)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(WorkingDirectory)

				CLASS_MEMBER_METHOD(GetDocumentByPath, {L"path"})
				CLASS_MEMBER_METHOD(GetImageByPath, {L"path"})
				CLASS_MEMBER_METHOD(GetXmlByPath, {L"path"})
				CLASS_MEMBER_METHOD(GetStringByPath, {L"path"})
			END_CLASS_MEMBER(GuiResource)

			BEGIN_CLASS_MEMBER(GuiResourcePathResolver)
				CLASS_MEMBER_CONSTRUCTOR(Ptr<GuiResourcePathResolver>(Ptr<GuiResource>, WString), {L"resource" _ L"workingDirectory"})

				CLASS_MEMBER_METHOD(ResolveResource, {L"protocol" _ L"path"})
			END_CLASS_MEMBER(GuiResourcePathResolver)

			BEGIN_ENUM_ITEM(GuiResourceUsage)
				ENUM_CLASS_ITEM(DataOnly)
				ENUM_CLASS_ITEM(InstanceClass)
			END_ENUM_ITEM(GuiResourceUsage)

				BEGIN_INTERFACE_MEMBER_NOPROXY(IGuiResourceManager)
					CLASS_MEMBER_STATIC_EXTERNALMETHOD(GetResourceManager, NO_PARAMETER, IGuiResourceManager*(*)(), vl::presentation::GetResourceManager)
				CLASS_MEMBER_METHOD(SetResource, { L"name" _ L"resource" _ L"usage" })
				CLASS_MEMBER_METHOD(GetResource, { L"name" })
				CLASS_MEMBER_METHOD(GetResourceFromClassName, { L"name" })
			END_INTERFACE_MEMBER(IGuiResourceManager)

			BEGIN_INTERFACE_MEMBER_NOPROXY(IGuiGraphicsElement)
			END_INTERFACE_MEMBER(IGuiGraphicsElement)

			BEGIN_CLASS_MEMBER(GuiGraphicsComposition)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(leftButtonDown)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(leftButtonUp)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(leftButtonDoubleClick)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(middleButtonDown)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(middleButtonUp)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(middleButtonDoubleClick)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(rightButtonDown)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(rightButtonUp)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(rightButtonDoubleClick)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(horizontalWheel)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(verticalWheel)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(mouseMove)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(mouseEnter)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(mouseLeave)
				
				CLASS_MEMBER_GUIEVENT_COMPOSITION(previewKey)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(keyDown)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(keyUp)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(systemKeyDown)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(systemKeyUp)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(previewCharInput)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(charInput)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(gotFocus)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(lostFocus)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(caretNotify)
				CLASS_MEMBER_GUIEVENT_COMPOSITION(clipboardNotify)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(Parent)
				CLASS_MEMBER_PROPERTY_FAST(OwnedElement)
				CLASS_MEMBER_PROPERTY_FAST(Visible)
				CLASS_MEMBER_PROPERTY_FAST(MinSizeLimitation)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(GlobalBounds)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(AssociatedControl)
				CLASS_MEMBER_PROPERTY_FAST(AssociatedCursor)
				CLASS_MEMBER_PROPERTY_FAST(AssociatedHitTestResult)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(RelatedControl)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(RelatedControlHost)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(RelatedCursor)
				CLASS_MEMBER_PROPERTY_FAST(Margin)
				CLASS_MEMBER_PROPERTY_FAST(InternalMargin)
				CLASS_MEMBER_PROPERTY_FAST(PreferredMinSize)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ClientArea)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(MinPreferredClientSize)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(PreferredBounds)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Bounds)

				CLASS_MEMBER_METHOD_RENAME(GetChildren, Children, NO_PARAMETER)
				CLASS_MEMBER_PROPERTY_READONLY(Children, GetChildren)

				CLASS_MEMBER_METHOD(AddChild, {L"child"})
				CLASS_MEMBER_METHOD(InsertChild, {L"index" _ L"child"})
				CLASS_MEMBER_METHOD(RemoveChild, {L"child"})
				CLASS_MEMBER_METHOD(MoveChild, {L"child" _ L"newIndex"})
				CLASS_MEMBER_METHOD(Render, {L"size"})
				CLASS_MEMBER_METHOD(FindComposition, {L"location"})
				CLASS_MEMBER_METHOD(ForceCalculateSizeImmediately, NO_PARAMETER)
				CLASS_MEMBER_METHOD(IsSizeAffectParent, NO_PARAMETER)
			END_CLASS_MEMBER(GuiGraphicsComposition)

			BEGIN_ENUM_ITEM(GuiGraphicsComposition::MinSizeLimitation)
				ENUM_ITEM_NAMESPACE(GuiGraphicsComposition)
				ENUM_NAMESPACE_ITEM(NoLimit)
				ENUM_NAMESPACE_ITEM(LimitToElement)
				ENUM_NAMESPACE_ITEM(LimitToElementAndChildren)
			END_ENUM_ITEM(GuiGraphicsComposition::MinSizeLimitation)

			BEGIN_ENUM_ITEM(INativeWindowListener::HitTestResult)
				ENUM_ITEM_NAMESPACE(INativeWindowListener)
				ENUM_NAMESPACE_ITEM(BorderNoSizing)
				ENUM_NAMESPACE_ITEM(BorderLeft)
				ENUM_NAMESPACE_ITEM(BorderRight)
				ENUM_NAMESPACE_ITEM(BorderTop)
				ENUM_NAMESPACE_ITEM(BorderBottom)
				ENUM_NAMESPACE_ITEM(BorderLeftTop)
				ENUM_NAMESPACE_ITEM(BorderRightTop)
				ENUM_NAMESPACE_ITEM(BorderLeftBottom)
				ENUM_NAMESPACE_ITEM(BorderRightBottom)
				ENUM_NAMESPACE_ITEM(Title)
				ENUM_NAMESPACE_ITEM(ButtonMinimum)
				ENUM_NAMESPACE_ITEM(ButtonMaximum)
				ENUM_NAMESPACE_ITEM(ButtonClose)
				ENUM_NAMESPACE_ITEM(Client)
				ENUM_NAMESPACE_ITEM(Icon)
				ENUM_NAMESPACE_ITEM(NoDecision)
			END_ENUM_ITEM(INativeWindowListener::HitTestResult)

			BEGIN_CLASS_MEMBER(GuiGraphicsSite)
				CLASS_MEMBER_BASE(GuiGraphicsComposition)

				CLASS_MEMBER_PROPERTY_GUIEVENT_READONLY_FAST(Bounds)
			END_CLASS_MEMBER(GuiGraphicsSite)

			BEGIN_CLASS_MEMBER(GuiWindowComposition)
				CLASS_MEMBER_BASE(GuiGraphicsSite)
				CLASS_MEMBER_CONSTRUCTOR(GuiWindowComposition*(), NO_PARAMETER)
			END_CLASS_MEMBER(GuiWindowComposition)

			BEGIN_CLASS_MEMBER(GuiBoundsComposition)
				CLASS_MEMBER_BASE(GuiGraphicsSite)
				CLASS_MEMBER_CONSTRUCTOR(GuiBoundsComposition*(), NO_PARAMETER)

				CLASS_MEMBER_PROPERTY_EVENT_FAST(Bounds, BoundsChanged)
				CLASS_MEMBER_PROPERTY_FAST(AlignmentToParent)
				
				CLASS_MEMBER_METHOD(ClearAlignmentToParent, NO_PARAMETER)
				CLASS_MEMBER_METHOD(IsAlignedToParent, NO_PARAMETER)
			END_CLASS_MEMBER(GuiBoundsComposition)

			BEGIN_CLASS_MEMBER(GuiControl)
				CLASS_MEMBER_CONSTRUCTOR(GuiControl*(GuiControl::IStyleController*), {L"styleController"})

				CLASS_MEMBER_PROPERTY_READONLY_FAST(StyleController)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(BoundsComposition)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ContainerComposition)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(FocusableComposition)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(Parent)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(ChildrenCount)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(RelatedControlHost)
				CLASS_MEMBER_PROPERTY_GUIEVENT_READONLY_FAST(VisuallyEnabled)
				CLASS_MEMBER_PROPERTY_GUIEVENT_FAST(Enabled)
				CLASS_MEMBER_PROPERTY_GUIEVENT_FAST(Visible)
				CLASS_MEMBER_PROPERTY_GUIEVENT_FAST(Alt)
				CLASS_MEMBER_PROPERTY_GUIEVENT_FAST(Text)
				CLASS_MEMBER_PROPERTY_GUIEVENT_FAST(Font)
				CLASS_MEMBER_PROPERTY_FAST(Tag)
				CLASS_MEMBER_PROPERTY_FAST(TooltipControl)
				CLASS_MEMBER_PROPERTY_FAST(TooltipWidth)

				CLASS_MEMBER_METHOD(SetActivatingAltHost, { L"host" })
				CLASS_MEMBER_METHOD(GetChild, {L"index"})
				CLASS_MEMBER_METHOD(AddChild, {L"control"})
				CLASS_MEMBER_METHOD(HasChild, {L"control"})
				CLASS_MEMBER_METHOD(SetFocus, NO_PARAMETER)
				CLASS_MEMBER_METHOD(DisplayTooltip, {L"location"})
				CLASS_MEMBER_METHOD(CloseTooltip, NO_PARAMETER)
				CLASS_MEMBER_METHOD_OVERLOAD(QueryService, {L"identifier"}, IDescriptable*(GuiControl::*)(const WString&))
			END_CLASS_MEMBER(GuiControl)

			BEGIN_INTERFACE_MEMBER(GuiControl::IStyleController)
				CLASS_MEMBER_METHOD(GetBoundsComposition, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetContainerComposition, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SetFocusableComposition, {L"value"})
				CLASS_MEMBER_METHOD(SetText, {L"value"})
				CLASS_MEMBER_METHOD(SetFont, {L"value"})
				CLASS_MEMBER_METHOD(SetVisuallyEnabled, {L"value"})
			END_INTERFACE_MEMBER(GuiControl::IStyleController)

			BEGIN_INTERFACE_MEMBER(GuiControl::IStyleProvider)
				CLASS_MEMBER_METHOD(AssociateStyleController, {L"controller"})
				CLASS_MEMBER_METHOD(SetFocusableComposition, {L"value"})
				CLASS_MEMBER_METHOD(SetText, {L"value"})
				CLASS_MEMBER_METHOD(SetFont, {L"value"})
				CLASS_MEMBER_METHOD(SetVisuallyEnabled, {L"value"})
			END_INTERFACE_MEMBER(GuiControl::IStyleProvider)

			BEGIN_CLASS_MEMBER(GuiComponent)
			END_CLASS_MEMBER(GuiComponent)

			BEGIN_CLASS_MEMBER(GuiControlHost)
				CLASS_MEMBER_BASE(GuiControl)
				CLASS_MEMBER_BASE(GuiInstanceRootObject)
				CLASS_MEMBER_CONSTRUCTOR(GuiControlHost*(GuiControl::IStyleController*), {L"styleController"})

				CLASS_MEMBER_GUIEVENT(WindowGotFocus)
				CLASS_MEMBER_GUIEVENT(WindowLostFocus)
				CLASS_MEMBER_GUIEVENT(WindowActivated)
				CLASS_MEMBER_GUIEVENT(WindowDeactivated)
				CLASS_MEMBER_GUIEVENT(WindowOpened)
				CLASS_MEMBER_GUIEVENT(WindowClosing)
				CLASS_MEMBER_GUIEVENT(WindowClosed)
				CLASS_MEMBER_GUIEVENT(WindowDestroying)

				CLASS_MEMBER_PROPERTY_READONLY_FAST(MainComposition)
				CLASS_MEMBER_PROPERTY_FAST(ShowInTaskBar)
				CLASS_MEMBER_PROPERTY_FAST(EnabledActivate)
				CLASS_MEMBER_PROPERTY_FAST(TopMost)
				CLASS_MEMBER_PROPERTY_FAST(ClientSize)
				CLASS_MEMBER_PROPERTY_FAST(Bounds)
				CLASS_MEMBER_PROPERTY_FAST(ShortcutKeyManager)
				CLASS_MEMBER_PROPERTY_READONLY_FAST(AnimationManager)

				CLASS_MEMBER_METHOD(ForceCalculateSizeImmediately, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetFocused, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SetFocused, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetActivated, NO_PARAMETER)
				CLASS_MEMBER_METHOD(SetActivated, NO_PARAMETER)
				CLASS_MEMBER_METHOD(Show, NO_PARAMETER)
				CLASS_MEMBER_METHOD(ShowDeactivated, NO_PARAMETER)
				CLASS_MEMBER_METHOD(ShowRestored, NO_PARAMETER)
				CLASS_MEMBER_METHOD(ShowMaximized, NO_PARAMETER)
				CLASS_MEMBER_METHOD(ShowMinimized, NO_PARAMETER)
				CLASS_MEMBER_METHOD(Hide, NO_PARAMETER)
				CLASS_MEMBER_METHOD(Close, NO_PARAMETER)
				CLASS_MEMBER_METHOD(GetOpening, NO_PARAMETER)
			END_CLASS_MEMBER(GuiControlHost)

#undef _

/***********************************************************************
Type Loader
***********************************************************************/

			class GuiBasicTypeLoader : public Object, public ITypeLoader
			{
			public:
				void Load(ITypeManager* manager)
				{
					GUIREFLECTIONBASIC_TYPELIST(ADD_TYPE_INFO)
				}

				void Unload(ITypeManager* manager)
				{
				}
			};

#endif

			bool LoadGuiBasicTypes()
			{
#ifndef VCZH_DEBUG_NO_REFLECTION
				ITypeManager* manager=GetGlobalTypeManager();
				if(manager)
				{
					Ptr<ITypeLoader> loader=new GuiBasicTypeLoader;
					return manager->AddTypeLoader(loader);
				}
#endif
				return false;
			}
		}
	}
}