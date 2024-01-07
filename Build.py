import subprocess
import argparse
import os
import platform

class CompilerInfo:
    def __init__(self) -> None:
        self._compilerName = ""
        self._compilerInstallPath = ""

class Command:
    def __init__(self) -> None:
        self._command = ""
    
    def AddCommandArg(self, arg):
        self._command += " "
        self._command += arg
    
    def Excute(self):
        subprocess.call(self._command)

class CMakeCommand(Command):
    def __init__(self) -> None:
        super().__init__()
        self._command = "cmake"

class MSBuildCommand(Command):
    def __init__(self) -> None:
        super().__init__()
        self._command = "msbuild"

class BuildBatch:
    def __init__(self) -> None:
        self._batches = {}
    
    def AddCMakeCommand(self, command):
        self._batches["cmake"] = command

    def AddMSBuildCommand(self, command):
        self._batches["msbuild"] = command
    
    def ExcuteCommand(self):
        # We need to excute the command by order
        # cmake --> msbuild --> others
        if "cmake" in self._batches:
            self._batches["cmake"].Excute()
        
        if "msbuild" in self._batches:
            self._batches["msbuild"].Excute()
        
        # Others

def FindProgramFilesFolder():
    env = os.environ
    if platform.architecture()[0] == "64bit":
        if "ProgramFiles(x86)" in env:
            programFilesFolder = env["ProgramFiles(x86)"]
        else:
            programFilesFolder = "C:\\Program Files (x86)"
    else:
        if "ProgramFiles" in env:
            programFilesFolder = env["ProgramFiles"]
        else:
            programFilesFolder = "C:\\Program Files"
    
    return programFilesFolder

def FindInstallCompiler(buildFilePath):
    if (os.path.exists(buildFilePath)):
        cmakeCacheFilePath = buildFilePath + "/CMakeCache.txt"
        print(cmakeCacheFilePath)
        with open(cmakeCacheFilePath, "r") as f:
            cacheContent = f.read()
            cacheContent = cacheContent.split("\n")

            comilerInfo = {}
            for line in cacheContent:
                if line.startswith("CMAKE_GENERATOR_INSTANCE:INTERNAL="):
                    comilerInfo["compilerPath"] = line[34 : ]
                    print(line)
                elif line.startswith("CMAKE_GENERATOR:INTERNAL="):
                    comilerInfo["compilerName"] = line[25 : ]
            
            if "compilerPath" in comilerInfo:
                return comilerInfo["compilerPath"]
            elif "compilerName" in comilerInfo:
                compiler = comilerInfo["comilerName"]
                versionNum = compiler[13:15]
                return FindVisualStudioInfo(versionNum)
    
    return None

def FindVisualStudioInfo(versionNum):
    vsWhereLocation = FindProgramFilesFolder() + "\\Microsoft Visual Studio\\Installer\\vswhere.exe"
    if os.path.exists(vsWhereLocation):
        vsInfo = subprocess.check_output([vsWhereLocation, "-products", "*", "-latest", "-property", "installationPath", "-version", "[%d.0,%d.0)" % (versionNum, versionNum + 1)]).decode()
        return vsInfo
    
    return None

def __main():
    parser = argparse.ArgumentParser()

    parser.add_argument("-m", "--mode", choices = ["make", "build"], required = True, metavar = "", help = "Choose the mode of the script")
    parser.add_argument("-sp", "--sourcePath", type = str, default = "./", metavar = "", help = "The source file path")
    parser.add_argument("-bp", "--buildFilePath", type = str, default = "./Build", metavar = "", help = "The path of generate build files path")
    parser.add_argument("-s", "--sample", type = str, choices = ["on", "off"], default = "on", metavar = "", help = "Toggle generate sample projects or not, the default is on")
    parser.add_argument("-e", "--editor", type = str, choices = ["on", "off"], default = "on", metavar = "", help = "Toggle generate editor projects or not, the default is on")

    args = parser.parse_args()

    batches = BuildBatch()
    if args.mode == "make":
        cmakeCommand = CMakeCommand()
        cmakeCommand.AddCommandArg(f"-S {args.sourcePath} -B {args.buildFilePath}")

        if args.sample == "off":
            cmakeCommand.AddCommandArg("-DGHOST_BUILD_SAMPLES=OFF")
        else:
            cmakeCommand.AddCommandArg("-DGHOST_BUILD_SAMPLES=ON")
        
        if args.editor == "off":
            cmakeCommand.AddCommandArg("-DGHOST_BUILD_EDITOR=OFF")
        else:
            cmakeCommand.AddCommandArg("-DGHOST_BUILD_EDITOR=ON")
        
        batches.AddCMakeCommand(cmakeCommand)

    elif args.mode == "build":
        msbuildCommand = MSBuildCommand()
        #print(FindInstallCompiler(args.buildFilePath))
        print(FindVisualStudioInfo(17))

        #batches.AddMSBuildCommand(msbuildCommand)
    
    batches.ExcuteCommand()

if __name__ == "__main__":
    __main()