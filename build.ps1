#setup
# Project Dir
#    └---> Obj
#    └---> Src
#    		└--> Include


# -------- BUILD SETTINGS --------

$out_file = "Pong.exe"
$src_files = "main", "text", "puck", "paddle", "utils"     # ALL
# $src_files = "main" 

$build_all = $true;


$C_FLAGS = "-Wall", "-Wextra", "-pedantic", "-std=c++20", "-O3"

# Linking (Dynamic)

# -mwindows  Window based (Not Console Based)
$LINKER_FLAGS = "-lmingw32", "-lSDL2main", "-lSDL2", "-lSDL2_ttf", "-s"



$include_path = "Src/Include"


# -- Dependancies --

# Include paths
$inc_sdl = "./Lib/SDL2/include/"
$inc_sdl_ttf = "./Lib/SDL2_ttf/include/"


# Library Paths
$lib_sdl = "./Lib/SDL2/lib/"
$lib_sdl_ttf = "./Lib/SDL2_ttf/lib/"


# -------- BUILD SCRIPT --------
# removing previous build
if (Test-Path ./$out_file) {
	Remove-Item $out_file
}


# Compiling
Write-Output "Building"

if (($build_all) -and ($src_files)) {
	if (!(Test-Path("./Obj"))) {
		mkdir "./Obj";
	}

	foreach ($file in $src_files) {
		Write-Output "    ${file}.cpp"

		if (Test-Path "Obj/${file}.o") {
			Remove-Item Obj/${file}.o
		}

		g++ $C_FLAGS -I $include_path -I $inc_sdl -I $inc_sdl_ttf -o Obj/${file}.o -c Src/${file}.cpp
	
	}
}


# Linking
Write-Output "Linking"
$obj_files = Get-ChildItem -Path Obj/
g++ $C_FLAGS $obj_files -L $lib_sdl -L $lib_sdl_ttf $LINKER_FLAGS -o $out_file


# Running current build
if (Test-Path ./$out_file) {
	Write-Output "Build Successfully"
	& ./$out_file
}
else {
	Write-Output "ERROR in Building!"
}


if (Test-Path ("./app.exe")) {
	Remove-Item app.exe
}
