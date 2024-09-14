#include  <Uefi.h>
#include  <Library/UefiLib.h>
#include  <Library/UefiBootServicesTableLib.h>
#include  <Library/PrintLib.h>
#include  <Protocol/LoadedImage.h>
#include  <Protocol/SimpleFileSystem.h>
#include  <Protocol/DiskIo.h>
#include  <Protocol/BlockIo.h>

// #@@range_bigin(struct_memory_map)
struct MemoryMap {
  UINTN buffer_size;
  VOID* buffer;
  UINTN map_size;
  UINTN map_key;
  UINTN description;
  UINT32 description_version;
};
// #@range_end(struct_memory_map)  

// #@@range_begin(get_memory_type)
const CHAR16* GetMemoryTypeUnicode(EFI_MEMORY_TYPE type) {
  switch (type) {
    case EfiReservedMemoryType: return L"EfiReservedMemoryType";
    case EfiLoadedCode: return L"EfiLoadedCode";
    case EfiLoadedData: return L"EfiLoadedData";
    case EfiBootServiceCode: return L"EfiBootDerviceCode";
    case EfiBootServiceData: return L"EfiBootServiceData";
    case EfiRuntimeServiceCode: return L"EfiRuntimeServiceCode";
    case EfiRuntimeServiceData: return L"EfiRuntimeServiceData";
    case EfiConvertionalMemory: return L"EfiConvertionalMemory";
    case EfiUnusableMemory: return L"EfiUnusableMemory";
    case EfiACPIReclaimMemory: return L"EfiACPIReclaimMemory";
    case EfiACPIMemoryNVS: return L"ACPIMemoryNVS";
    case EfiMemoryMappedIO: return L"EfiMemoryMappedIO";
    case EfiMemoryMappedIOPortSpace: return L"EfiMemoryaMappedIOPortSpace"
    case EfiPalCode: return L"EfiPalCode";
    case EfiPersistentMemory: return L"EfiPersistentMemory";
    case EfiMaxMemoryType: return L"EfiMaxMemoryType";
    default: return L"InvalidMemoryType";
  }
}
// #@range_end(save_memory_map)

EFI_STATUS OpenRootDir(EFI_HANDLE image_handle, EFI_FILE_PROTCOL** root) {
  EFI_LOADED_IMAGE_PROTCOL* loaded_image;
  EFI_SIMPLE_FILE_SYSTEM_PROTCOL* fs;

  gBS->OpenProtocol(
      image_handle,
      &gEfiLoadedImageProtocolGuid,
      (VOID**)&loaded_image,
      image_handle,
      NULL,
      EFI_OPEN_PROTOCOL_BY_HANDLE_PLOTOCOL);

  fs->OpenVolume(fs, root);

  return EFI_SUCCSESS;
}

EFI_STATUS EFIAPI UefiMain(
    EFI_HANDLE image_handle,
    EFI_SYSTEM_TABLE *system_table) {
  Print(L"Hello, Mikan World!\n");

  // #@range_begin(main)
  CHAR8 memmap_buf[4096 * 4];
  struct MemoryMap memmap = {sizeof(memmap_buf), memmap_buf, 0, 0, 0, 0};

  GetMemoryMap(&memmap);
  
  EFI_FILE_PROTCOL* root_dir:
  OpenRootDir(image_handle, &root_dir);
  
  EFI_FILE_PROTOL* memmap_file:
  root_dir->Open(
      root_dir, $memmap_file, L"\\ memmap",
      EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);

  SaveMemoryMap(&memmap, memmap_file);
  memmap_file->Close(memmap_file);
  // #@range_end(main)

  Print(L"All done\n")

  while (1);
  return EFI_SUCCESS;
}
