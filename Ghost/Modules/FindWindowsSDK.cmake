# -----------------------------------------------------------------------------
# FindWindowsSDK.cmake
#
# Locates the Windows 10 SDK once and exposes the DirectX headers/libraries
# used by the engine.
#
# Defines:
#   WindowsSDK_FOUND
#   WindowsSDK_VERSION
#   WindowsSDK_INCLUDE_DIRS
#   DirectX11_LIBRARIES   (d3d11 + shared DXGI/D3DCompiler libs)
#   DirectX12_LIBRARIES   (d3d12 + shared DXGI/D3DCompiler libs)
#   DirectX_LIBRARIES     (both APIs, shared libs listed once)

if(NOT WIN32)
    message(FATAL_ERROR "FindWindowsSDK: only supported on Windows.")
endif()

# Windows 10 SDK root, registered by the SDK installer.
get_filename_component(WINSDK_ROOT
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot10]"
    REALPATH)

# Prefer the SDK version selected by the generator (VS 2017/2019/2022).
if(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION)
    set(WindowsSDK_VERSION ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION})
else()
    file(GLOB _winsdk_versions RELATIVE "${WINSDK_ROOT}/Include" "${WINSDK_ROOT}/Include/10.*")
    list(SORT _winsdk_versions)
    list(REVERSE _winsdk_versions)
    if(_winsdk_versions)
        list(GET _winsdk_versions 0 WindowsSDK_VERSION)
    endif()
endif()

# Probe with d3d12.h: it guarantees a Windows 10+ SDK, which also contains all
# the D3D11 headers we need.
find_path(WindowsSDK_INCLUDE_DIR
    NAMES d3d12.h
    HINTS
        "${WINSDK_ROOT}/Include/${WindowsSDK_VERSION}/um"
        "C:/Program Files (x86)/Windows Kits/10/Include/${WindowsSDK_VERSION}/um"
        "C:/Program Files/Windows Kits/10/Include/${WindowsSDK_VERSION}/um")

# These libraries ship with the Windows 10 SDK. With the Visual Studio
# generator the SDK lib directory is already on the linker search path, so
# bare library names are enough.
set(_winsdk_shared_libs dxgi.lib dxguid.lib d3dcompiler.lib)
set(DirectX11_LIBRARIES d3d11.lib ${_winsdk_shared_libs})
set(DirectX12_LIBRARIES d3d12.lib ${_winsdk_shared_libs})
set(DirectX_LIBRARIES d3d11.lib d3d12.lib ${_winsdk_shared_libs})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(WindowsSDK
    REQUIRED_VARS WindowsSDK_INCLUDE_DIR
    VERSION_VAR WindowsSDK_VERSION)

if(WindowsSDK_FOUND)
    set(WindowsSDK_INCLUDE_DIRS "${WindowsSDK_INCLUDE_DIR}")
    message(STATUS "Found Windows SDK ${WindowsSDK_VERSION}: ${WindowsSDK_INCLUDE_DIR}")
endif()

mark_as_advanced(WindowsSDK_INCLUDE_DIR)
