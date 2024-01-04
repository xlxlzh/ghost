import subprocess
import argparse

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

def __main():
    parser = argparse.ArgumentParser()

    parser.add_argument("-m", "--mode", choices = ["make", "build"], required = True, metavar = "", help = "Choose the mode of the script")
    parser.add_argument("-sp", "--sourcePath", type = str, default = "./", metavar = "", help = "The source file path")
    parser.add_argument("-bp", "--buildFilePath", type = str, default = "./Build", metavar = "", help = "The path of generate build files path")
    parser.add_argument("-s", "--sample", type = str, choices = ["on", "off"], default = "on", metavar = "", help = "Toggle generate sample projects or not, the default is on")

    args = parser.parse_args()

    batches = BuildBatch()
    if args.mode == "make":
        cmakeCommand = CMakeCommand()
        cmakeCommand.AddCommandArg(f"-S {args.sourcePath} -B {args.buildFilePath}")
        if args.sample == "off":
            cmakeCommand.AddCommandArg("-DGHOST_BUILD_SAMPLES=OFF")
        else:
            cmakeCommand.AddCommandArg("-DGHOST_BUILD_SAMPLES=ON")
        
        batches.AddCMakeCommand(cmakeCommand)
    
    batches.ExcuteCommand()

if __name__ == "__main__":
    __main()