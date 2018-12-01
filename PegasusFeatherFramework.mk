##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=PegasusFeatherFramework
ConfigurationName      :=Release
WorkspacePath          :=/home/ripper/Documents/shadesprototype
ProjectPath            :=/home/ripper/Documents/shadesprototype
IntermediateDirectory  :=./bin
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=ripper
Date                   :=12/02/18
CodeLitePath           :=/home/ripper/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="PegasusFeatherFramework.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  `sdl2-config --libs` `xml2-config --libs` 
IncludePath            :=  $(IncludeSwitch)./include/ $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)GLEW $(LibrarySwitch)GL $(LibrarySwitch)SDL2_mixer $(LibrarySwitch)SDL2_ttf $(LibrarySwitch)SDL2_net $(LibrarySwitch)SDL2_image 
ArLibs                 :=  "GLEW" "GL" "SDL2_mixer" "SDL2_ttf" "SDL2_net" "SDL2_image" 
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS := `xml2-config --cflags` `sdl2-config --cflags`  -O2 -Wall $(Preprocessors)
CFLAGS   :=  -O2 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_VulkanDisplayManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SoundManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_InputManagerSDL.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_OpenGL1DisplayManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_InputManagerUWP.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SceneManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Scene.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_TileManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_UnitManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_Camera.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_InputManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_PackManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_MainMenuScene.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_D3D11DisplayManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Resource.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Actor.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Projectile.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_HUD.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_TextLibrary.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_Particle.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Button.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Texture.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_CollectibleSoul.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_AnimatedTexture.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_NetworkTestScene.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Vertex.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_tinyxml2.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Unit.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Matrix.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_Cursor.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_ObjectManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_UIElement.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_NetworkManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_ParticleManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Mesh.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_GameScene.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Player.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Tile.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_StartScreen.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_GameInfo.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Vector.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Object.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_TextLibrarySDL.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_FloatingText.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_DisplayManager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Building.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./bin || $(MakeDirCommand) ./bin


$(IntermediateDirectory)/.d:
	@test -d ./bin || $(MakeDirCommand) ./bin

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_VulkanDisplayManager.cpp$(ObjectSuffix): src/VulkanDisplayManager.cpp $(IntermediateDirectory)/src_VulkanDisplayManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/VulkanDisplayManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_VulkanDisplayManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_VulkanDisplayManager.cpp$(DependSuffix): src/VulkanDisplayManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_VulkanDisplayManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_VulkanDisplayManager.cpp$(DependSuffix) -MM src/VulkanDisplayManager.cpp

$(IntermediateDirectory)/src_VulkanDisplayManager.cpp$(PreprocessSuffix): src/VulkanDisplayManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_VulkanDisplayManager.cpp$(PreprocessSuffix) src/VulkanDisplayManager.cpp

$(IntermediateDirectory)/src_SoundManager.cpp$(ObjectSuffix): src/SoundManager.cpp $(IntermediateDirectory)/src_SoundManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/SoundManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SoundManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SoundManager.cpp$(DependSuffix): src/SoundManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SoundManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SoundManager.cpp$(DependSuffix) -MM src/SoundManager.cpp

$(IntermediateDirectory)/src_SoundManager.cpp$(PreprocessSuffix): src/SoundManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SoundManager.cpp$(PreprocessSuffix) src/SoundManager.cpp

$(IntermediateDirectory)/src_InputManagerSDL.cpp$(ObjectSuffix): src/InputManagerSDL.cpp $(IntermediateDirectory)/src_InputManagerSDL.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/InputManagerSDL.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_InputManagerSDL.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_InputManagerSDL.cpp$(DependSuffix): src/InputManagerSDL.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_InputManagerSDL.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_InputManagerSDL.cpp$(DependSuffix) -MM src/InputManagerSDL.cpp

$(IntermediateDirectory)/src_InputManagerSDL.cpp$(PreprocessSuffix): src/InputManagerSDL.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_InputManagerSDL.cpp$(PreprocessSuffix) src/InputManagerSDL.cpp

$(IntermediateDirectory)/src_OpenGL1DisplayManager.cpp$(ObjectSuffix): src/OpenGL1DisplayManager.cpp $(IntermediateDirectory)/src_OpenGL1DisplayManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/OpenGL1DisplayManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_OpenGL1DisplayManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_OpenGL1DisplayManager.cpp$(DependSuffix): src/OpenGL1DisplayManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_OpenGL1DisplayManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_OpenGL1DisplayManager.cpp$(DependSuffix) -MM src/OpenGL1DisplayManager.cpp

$(IntermediateDirectory)/src_OpenGL1DisplayManager.cpp$(PreprocessSuffix): src/OpenGL1DisplayManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_OpenGL1DisplayManager.cpp$(PreprocessSuffix) src/OpenGL1DisplayManager.cpp

$(IntermediateDirectory)/src_InputManagerUWP.cpp$(ObjectSuffix): src/InputManagerUWP.cpp $(IntermediateDirectory)/src_InputManagerUWP.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/InputManagerUWP.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_InputManagerUWP.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_InputManagerUWP.cpp$(DependSuffix): src/InputManagerUWP.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_InputManagerUWP.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_InputManagerUWP.cpp$(DependSuffix) -MM src/InputManagerUWP.cpp

$(IntermediateDirectory)/src_InputManagerUWP.cpp$(PreprocessSuffix): src/InputManagerUWP.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_InputManagerUWP.cpp$(PreprocessSuffix) src/InputManagerUWP.cpp

$(IntermediateDirectory)/src_SceneManager.cpp$(ObjectSuffix): src/SceneManager.cpp $(IntermediateDirectory)/src_SceneManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/SceneManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SceneManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SceneManager.cpp$(DependSuffix): src/SceneManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SceneManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SceneManager.cpp$(DependSuffix) -MM src/SceneManager.cpp

$(IntermediateDirectory)/src_SceneManager.cpp$(PreprocessSuffix): src/SceneManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SceneManager.cpp$(PreprocessSuffix) src/SceneManager.cpp

$(IntermediateDirectory)/src_Scene.cpp$(ObjectSuffix): src/Scene.cpp $(IntermediateDirectory)/src_Scene.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/Scene.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Scene.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Scene.cpp$(DependSuffix): src/Scene.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Scene.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Scene.cpp$(DependSuffix) -MM src/Scene.cpp

$(IntermediateDirectory)/src_Scene.cpp$(PreprocessSuffix): src/Scene.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Scene.cpp$(PreprocessSuffix) src/Scene.cpp

$(IntermediateDirectory)/src_TileManager.cpp$(ObjectSuffix): src/TileManager.cpp $(IntermediateDirectory)/src_TileManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/TileManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_TileManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_TileManager.cpp$(DependSuffix): src/TileManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_TileManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_TileManager.cpp$(DependSuffix) -MM src/TileManager.cpp

$(IntermediateDirectory)/src_TileManager.cpp$(PreprocessSuffix): src/TileManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_TileManager.cpp$(PreprocessSuffix) src/TileManager.cpp

$(IntermediateDirectory)/src_UnitManager.cpp$(ObjectSuffix): src/UnitManager.cpp $(IntermediateDirectory)/src_UnitManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/UnitManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_UnitManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_UnitManager.cpp$(DependSuffix): src/UnitManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_UnitManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_UnitManager.cpp$(DependSuffix) -MM src/UnitManager.cpp

$(IntermediateDirectory)/src_UnitManager.cpp$(PreprocessSuffix): src/UnitManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_UnitManager.cpp$(PreprocessSuffix) src/UnitManager.cpp

$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix): src/main.cpp $(IntermediateDirectory)/src_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_main.cpp$(DependSuffix): src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_main.cpp$(DependSuffix) -MM src/main.cpp

$(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix): src/main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix) src/main.cpp

$(IntermediateDirectory)/src_Camera.cpp$(ObjectSuffix): src/Camera.cpp $(IntermediateDirectory)/src_Camera.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/Camera.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Camera.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Camera.cpp$(DependSuffix): src/Camera.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Camera.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Camera.cpp$(DependSuffix) -MM src/Camera.cpp

$(IntermediateDirectory)/src_Camera.cpp$(PreprocessSuffix): src/Camera.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Camera.cpp$(PreprocessSuffix) src/Camera.cpp

$(IntermediateDirectory)/src_InputManager.cpp$(ObjectSuffix): src/InputManager.cpp $(IntermediateDirectory)/src_InputManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/InputManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_InputManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_InputManager.cpp$(DependSuffix): src/InputManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_InputManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_InputManager.cpp$(DependSuffix) -MM src/InputManager.cpp

$(IntermediateDirectory)/src_InputManager.cpp$(PreprocessSuffix): src/InputManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_InputManager.cpp$(PreprocessSuffix) src/InputManager.cpp

$(IntermediateDirectory)/src_PackManager.cpp$(ObjectSuffix): src/PackManager.cpp $(IntermediateDirectory)/src_PackManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/PackManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_PackManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_PackManager.cpp$(DependSuffix): src/PackManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_PackManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_PackManager.cpp$(DependSuffix) -MM src/PackManager.cpp

$(IntermediateDirectory)/src_PackManager.cpp$(PreprocessSuffix): src/PackManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_PackManager.cpp$(PreprocessSuffix) src/PackManager.cpp

$(IntermediateDirectory)/src_MainMenuScene.cpp$(ObjectSuffix): src/MainMenuScene.cpp $(IntermediateDirectory)/src_MainMenuScene.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/MainMenuScene.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_MainMenuScene.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_MainMenuScene.cpp$(DependSuffix): src/MainMenuScene.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_MainMenuScene.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_MainMenuScene.cpp$(DependSuffix) -MM src/MainMenuScene.cpp

$(IntermediateDirectory)/src_MainMenuScene.cpp$(PreprocessSuffix): src/MainMenuScene.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_MainMenuScene.cpp$(PreprocessSuffix) src/MainMenuScene.cpp

$(IntermediateDirectory)/src_D3D11DisplayManager.cpp$(ObjectSuffix): src/D3D11DisplayManager.cpp $(IntermediateDirectory)/src_D3D11DisplayManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/D3D11DisplayManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_D3D11DisplayManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_D3D11DisplayManager.cpp$(DependSuffix): src/D3D11DisplayManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_D3D11DisplayManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_D3D11DisplayManager.cpp$(DependSuffix) -MM src/D3D11DisplayManager.cpp

$(IntermediateDirectory)/src_D3D11DisplayManager.cpp$(PreprocessSuffix): src/D3D11DisplayManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_D3D11DisplayManager.cpp$(PreprocessSuffix) src/D3D11DisplayManager.cpp

$(IntermediateDirectory)/src_Resource.cpp$(ObjectSuffix): src/Resource.cpp $(IntermediateDirectory)/src_Resource.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/Resource.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Resource.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Resource.cpp$(DependSuffix): src/Resource.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Resource.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Resource.cpp$(DependSuffix) -MM src/Resource.cpp

$(IntermediateDirectory)/src_Resource.cpp$(PreprocessSuffix): src/Resource.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Resource.cpp$(PreprocessSuffix) src/Resource.cpp

$(IntermediateDirectory)/src_Actor.cpp$(ObjectSuffix): src/Actor.cpp $(IntermediateDirectory)/src_Actor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/Actor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Actor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Actor.cpp$(DependSuffix): src/Actor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Actor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Actor.cpp$(DependSuffix) -MM src/Actor.cpp

$(IntermediateDirectory)/src_Actor.cpp$(PreprocessSuffix): src/Actor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Actor.cpp$(PreprocessSuffix) src/Actor.cpp

$(IntermediateDirectory)/src_Projectile.cpp$(ObjectSuffix): src/Projectile.cpp $(IntermediateDirectory)/src_Projectile.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/Projectile.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Projectile.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Projectile.cpp$(DependSuffix): src/Projectile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Projectile.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Projectile.cpp$(DependSuffix) -MM src/Projectile.cpp

$(IntermediateDirectory)/src_Projectile.cpp$(PreprocessSuffix): src/Projectile.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Projectile.cpp$(PreprocessSuffix) src/Projectile.cpp

$(IntermediateDirectory)/src_HUD.cpp$(ObjectSuffix): src/HUD.cpp $(IntermediateDirectory)/src_HUD.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/HUD.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_HUD.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_HUD.cpp$(DependSuffix): src/HUD.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_HUD.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_HUD.cpp$(DependSuffix) -MM src/HUD.cpp

$(IntermediateDirectory)/src_HUD.cpp$(PreprocessSuffix): src/HUD.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_HUD.cpp$(PreprocessSuffix) src/HUD.cpp

$(IntermediateDirectory)/src_TextLibrary.cpp$(ObjectSuffix): src/TextLibrary.cpp $(IntermediateDirectory)/src_TextLibrary.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/TextLibrary.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_TextLibrary.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_TextLibrary.cpp$(DependSuffix): src/TextLibrary.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_TextLibrary.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_TextLibrary.cpp$(DependSuffix) -MM src/TextLibrary.cpp

$(IntermediateDirectory)/src_TextLibrary.cpp$(PreprocessSuffix): src/TextLibrary.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_TextLibrary.cpp$(PreprocessSuffix) src/TextLibrary.cpp

$(IntermediateDirectory)/src_Particle.cpp$(ObjectSuffix): src/Particle.cpp $(IntermediateDirectory)/src_Particle.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/Particle.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Particle.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Particle.cpp$(DependSuffix): src/Particle.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Particle.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Particle.cpp$(DependSuffix) -MM src/Particle.cpp

$(IntermediateDirectory)/src_Particle.cpp$(PreprocessSuffix): src/Particle.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Particle.cpp$(PreprocessSuffix) src/Particle.cpp

$(IntermediateDirectory)/src_Button.cpp$(ObjectSuffix): src/Button.cpp $(IntermediateDirectory)/src_Button.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/Button.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Button.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Button.cpp$(DependSuffix): src/Button.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Button.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Button.cpp$(DependSuffix) -MM src/Button.cpp

$(IntermediateDirectory)/src_Button.cpp$(PreprocessSuffix): src/Button.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Button.cpp$(PreprocessSuffix) src/Button.cpp

$(IntermediateDirectory)/src_Texture.cpp$(ObjectSuffix): src/Texture.cpp $(IntermediateDirectory)/src_Texture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/Texture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Texture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Texture.cpp$(DependSuffix): src/Texture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Texture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Texture.cpp$(DependSuffix) -MM src/Texture.cpp

$(IntermediateDirectory)/src_Texture.cpp$(PreprocessSuffix): src/Texture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Texture.cpp$(PreprocessSuffix) src/Texture.cpp

$(IntermediateDirectory)/src_CollectibleSoul.cpp$(ObjectSuffix): src/CollectibleSoul.cpp $(IntermediateDirectory)/src_CollectibleSoul.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/CollectibleSoul.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_CollectibleSoul.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_CollectibleSoul.cpp$(DependSuffix): src/CollectibleSoul.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_CollectibleSoul.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_CollectibleSoul.cpp$(DependSuffix) -MM src/CollectibleSoul.cpp

$(IntermediateDirectory)/src_CollectibleSoul.cpp$(PreprocessSuffix): src/CollectibleSoul.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_CollectibleSoul.cpp$(PreprocessSuffix) src/CollectibleSoul.cpp

$(IntermediateDirectory)/src_AnimatedTexture.cpp$(ObjectSuffix): src/AnimatedTexture.cpp $(IntermediateDirectory)/src_AnimatedTexture.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/AnimatedTexture.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_AnimatedTexture.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_AnimatedTexture.cpp$(DependSuffix): src/AnimatedTexture.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_AnimatedTexture.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_AnimatedTexture.cpp$(DependSuffix) -MM src/AnimatedTexture.cpp

$(IntermediateDirectory)/src_AnimatedTexture.cpp$(PreprocessSuffix): src/AnimatedTexture.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_AnimatedTexture.cpp$(PreprocessSuffix) src/AnimatedTexture.cpp

$(IntermediateDirectory)/src_NetworkTestScene.cpp$(ObjectSuffix): src/NetworkTestScene.cpp $(IntermediateDirectory)/src_NetworkTestScene.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/NetworkTestScene.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_NetworkTestScene.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_NetworkTestScene.cpp$(DependSuffix): src/NetworkTestScene.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_NetworkTestScene.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_NetworkTestScene.cpp$(DependSuffix) -MM src/NetworkTestScene.cpp

$(IntermediateDirectory)/src_NetworkTestScene.cpp$(PreprocessSuffix): src/NetworkTestScene.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_NetworkTestScene.cpp$(PreprocessSuffix) src/NetworkTestScene.cpp

$(IntermediateDirectory)/src_Vertex.cpp$(ObjectSuffix): src/Vertex.cpp $(IntermediateDirectory)/src_Vertex.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/Vertex.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Vertex.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Vertex.cpp$(DependSuffix): src/Vertex.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Vertex.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Vertex.cpp$(DependSuffix) -MM src/Vertex.cpp

$(IntermediateDirectory)/src_Vertex.cpp$(PreprocessSuffix): src/Vertex.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Vertex.cpp$(PreprocessSuffix) src/Vertex.cpp

$(IntermediateDirectory)/src_tinyxml2.cpp$(ObjectSuffix): src/tinyxml2.cpp $(IntermediateDirectory)/src_tinyxml2.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/tinyxml2.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_tinyxml2.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_tinyxml2.cpp$(DependSuffix): src/tinyxml2.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_tinyxml2.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_tinyxml2.cpp$(DependSuffix) -MM src/tinyxml2.cpp

$(IntermediateDirectory)/src_tinyxml2.cpp$(PreprocessSuffix): src/tinyxml2.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_tinyxml2.cpp$(PreprocessSuffix) src/tinyxml2.cpp

$(IntermediateDirectory)/src_Unit.cpp$(ObjectSuffix): src/Unit.cpp $(IntermediateDirectory)/src_Unit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/Unit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Unit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Unit.cpp$(DependSuffix): src/Unit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Unit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Unit.cpp$(DependSuffix) -MM src/Unit.cpp

$(IntermediateDirectory)/src_Unit.cpp$(PreprocessSuffix): src/Unit.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Unit.cpp$(PreprocessSuffix) src/Unit.cpp

$(IntermediateDirectory)/src_Matrix.cpp$(ObjectSuffix): src/Matrix.cpp $(IntermediateDirectory)/src_Matrix.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/Matrix.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Matrix.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Matrix.cpp$(DependSuffix): src/Matrix.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Matrix.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Matrix.cpp$(DependSuffix) -MM src/Matrix.cpp

$(IntermediateDirectory)/src_Matrix.cpp$(PreprocessSuffix): src/Matrix.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Matrix.cpp$(PreprocessSuffix) src/Matrix.cpp

$(IntermediateDirectory)/src_Cursor.cpp$(ObjectSuffix): src/Cursor.cpp $(IntermediateDirectory)/src_Cursor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/Cursor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Cursor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Cursor.cpp$(DependSuffix): src/Cursor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Cursor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Cursor.cpp$(DependSuffix) -MM src/Cursor.cpp

$(IntermediateDirectory)/src_Cursor.cpp$(PreprocessSuffix): src/Cursor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Cursor.cpp$(PreprocessSuffix) src/Cursor.cpp

$(IntermediateDirectory)/src_ObjectManager.cpp$(ObjectSuffix): src/ObjectManager.cpp $(IntermediateDirectory)/src_ObjectManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/ObjectManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_ObjectManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_ObjectManager.cpp$(DependSuffix): src/ObjectManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_ObjectManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_ObjectManager.cpp$(DependSuffix) -MM src/ObjectManager.cpp

$(IntermediateDirectory)/src_ObjectManager.cpp$(PreprocessSuffix): src/ObjectManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_ObjectManager.cpp$(PreprocessSuffix) src/ObjectManager.cpp

$(IntermediateDirectory)/src_UIElement.cpp$(ObjectSuffix): src/UIElement.cpp $(IntermediateDirectory)/src_UIElement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/UIElement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_UIElement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_UIElement.cpp$(DependSuffix): src/UIElement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_UIElement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_UIElement.cpp$(DependSuffix) -MM src/UIElement.cpp

$(IntermediateDirectory)/src_UIElement.cpp$(PreprocessSuffix): src/UIElement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_UIElement.cpp$(PreprocessSuffix) src/UIElement.cpp

$(IntermediateDirectory)/src_NetworkManager.cpp$(ObjectSuffix): src/NetworkManager.cpp $(IntermediateDirectory)/src_NetworkManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/NetworkManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_NetworkManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_NetworkManager.cpp$(DependSuffix): src/NetworkManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_NetworkManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_NetworkManager.cpp$(DependSuffix) -MM src/NetworkManager.cpp

$(IntermediateDirectory)/src_NetworkManager.cpp$(PreprocessSuffix): src/NetworkManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_NetworkManager.cpp$(PreprocessSuffix) src/NetworkManager.cpp

$(IntermediateDirectory)/src_ParticleManager.cpp$(ObjectSuffix): src/ParticleManager.cpp $(IntermediateDirectory)/src_ParticleManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/ParticleManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_ParticleManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_ParticleManager.cpp$(DependSuffix): src/ParticleManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_ParticleManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_ParticleManager.cpp$(DependSuffix) -MM src/ParticleManager.cpp

$(IntermediateDirectory)/src_ParticleManager.cpp$(PreprocessSuffix): src/ParticleManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_ParticleManager.cpp$(PreprocessSuffix) src/ParticleManager.cpp

$(IntermediateDirectory)/src_Mesh.cpp$(ObjectSuffix): src/Mesh.cpp $(IntermediateDirectory)/src_Mesh.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/Mesh.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Mesh.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Mesh.cpp$(DependSuffix): src/Mesh.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Mesh.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Mesh.cpp$(DependSuffix) -MM src/Mesh.cpp

$(IntermediateDirectory)/src_Mesh.cpp$(PreprocessSuffix): src/Mesh.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Mesh.cpp$(PreprocessSuffix) src/Mesh.cpp

$(IntermediateDirectory)/src_GameScene.cpp$(ObjectSuffix): src/GameScene.cpp $(IntermediateDirectory)/src_GameScene.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/GameScene.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_GameScene.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_GameScene.cpp$(DependSuffix): src/GameScene.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_GameScene.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_GameScene.cpp$(DependSuffix) -MM src/GameScene.cpp

$(IntermediateDirectory)/src_GameScene.cpp$(PreprocessSuffix): src/GameScene.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_GameScene.cpp$(PreprocessSuffix) src/GameScene.cpp

$(IntermediateDirectory)/src_Player.cpp$(ObjectSuffix): src/Player.cpp $(IntermediateDirectory)/src_Player.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/Player.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Player.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Player.cpp$(DependSuffix): src/Player.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Player.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Player.cpp$(DependSuffix) -MM src/Player.cpp

$(IntermediateDirectory)/src_Player.cpp$(PreprocessSuffix): src/Player.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Player.cpp$(PreprocessSuffix) src/Player.cpp

$(IntermediateDirectory)/src_Tile.cpp$(ObjectSuffix): src/Tile.cpp $(IntermediateDirectory)/src_Tile.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/Tile.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Tile.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Tile.cpp$(DependSuffix): src/Tile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Tile.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Tile.cpp$(DependSuffix) -MM src/Tile.cpp

$(IntermediateDirectory)/src_Tile.cpp$(PreprocessSuffix): src/Tile.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Tile.cpp$(PreprocessSuffix) src/Tile.cpp

$(IntermediateDirectory)/src_StartScreen.cpp$(ObjectSuffix): src/StartScreen.cpp $(IntermediateDirectory)/src_StartScreen.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/StartScreen.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_StartScreen.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_StartScreen.cpp$(DependSuffix): src/StartScreen.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_StartScreen.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_StartScreen.cpp$(DependSuffix) -MM src/StartScreen.cpp

$(IntermediateDirectory)/src_StartScreen.cpp$(PreprocessSuffix): src/StartScreen.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_StartScreen.cpp$(PreprocessSuffix) src/StartScreen.cpp

$(IntermediateDirectory)/src_GameInfo.cpp$(ObjectSuffix): src/GameInfo.cpp $(IntermediateDirectory)/src_GameInfo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/GameInfo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_GameInfo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_GameInfo.cpp$(DependSuffix): src/GameInfo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_GameInfo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_GameInfo.cpp$(DependSuffix) -MM src/GameInfo.cpp

$(IntermediateDirectory)/src_GameInfo.cpp$(PreprocessSuffix): src/GameInfo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_GameInfo.cpp$(PreprocessSuffix) src/GameInfo.cpp

$(IntermediateDirectory)/src_Vector.cpp$(ObjectSuffix): src/Vector.cpp $(IntermediateDirectory)/src_Vector.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/Vector.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Vector.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Vector.cpp$(DependSuffix): src/Vector.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Vector.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Vector.cpp$(DependSuffix) -MM src/Vector.cpp

$(IntermediateDirectory)/src_Vector.cpp$(PreprocessSuffix): src/Vector.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Vector.cpp$(PreprocessSuffix) src/Vector.cpp

$(IntermediateDirectory)/src_Object.cpp$(ObjectSuffix): src/Object.cpp $(IntermediateDirectory)/src_Object.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/Object.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Object.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Object.cpp$(DependSuffix): src/Object.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Object.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Object.cpp$(DependSuffix) -MM src/Object.cpp

$(IntermediateDirectory)/src_Object.cpp$(PreprocessSuffix): src/Object.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Object.cpp$(PreprocessSuffix) src/Object.cpp

$(IntermediateDirectory)/src_TextLibrarySDL.cpp$(ObjectSuffix): src/TextLibrarySDL.cpp $(IntermediateDirectory)/src_TextLibrarySDL.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/TextLibrarySDL.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_TextLibrarySDL.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_TextLibrarySDL.cpp$(DependSuffix): src/TextLibrarySDL.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_TextLibrarySDL.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_TextLibrarySDL.cpp$(DependSuffix) -MM src/TextLibrarySDL.cpp

$(IntermediateDirectory)/src_TextLibrarySDL.cpp$(PreprocessSuffix): src/TextLibrarySDL.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_TextLibrarySDL.cpp$(PreprocessSuffix) src/TextLibrarySDL.cpp

$(IntermediateDirectory)/src_FloatingText.cpp$(ObjectSuffix): src/FloatingText.cpp $(IntermediateDirectory)/src_FloatingText.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/FloatingText.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_FloatingText.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_FloatingText.cpp$(DependSuffix): src/FloatingText.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_FloatingText.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_FloatingText.cpp$(DependSuffix) -MM src/FloatingText.cpp

$(IntermediateDirectory)/src_FloatingText.cpp$(PreprocessSuffix): src/FloatingText.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_FloatingText.cpp$(PreprocessSuffix) src/FloatingText.cpp

$(IntermediateDirectory)/src_DisplayManager.cpp$(ObjectSuffix): src/DisplayManager.cpp $(IntermediateDirectory)/src_DisplayManager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/DisplayManager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_DisplayManager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_DisplayManager.cpp$(DependSuffix): src/DisplayManager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_DisplayManager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_DisplayManager.cpp$(DependSuffix) -MM src/DisplayManager.cpp

$(IntermediateDirectory)/src_DisplayManager.cpp$(PreprocessSuffix): src/DisplayManager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_DisplayManager.cpp$(PreprocessSuffix) src/DisplayManager.cpp

$(IntermediateDirectory)/src_Building.cpp$(ObjectSuffix): src/Building.cpp $(IntermediateDirectory)/src_Building.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ripper/Documents/shadesprototype/src/Building.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Building.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Building.cpp$(DependSuffix): src/Building.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Building.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Building.cpp$(DependSuffix) -MM src/Building.cpp

$(IntermediateDirectory)/src_Building.cpp$(PreprocessSuffix): src/Building.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Building.cpp$(PreprocessSuffix) src/Building.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./bin/


