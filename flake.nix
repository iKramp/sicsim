{
  description = "SIC/XE simulator";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      ...
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };

        imgui = builtins.fetchGit {
          url = "https://github.com/ocornut/imgui.git";
          rev = "33d9ed0d6bdeaf2fdd1636b72d68c268e851033f";
        };

        glfw = builtins.fetchGit {
          url = "https://github.com/glfw/glfw.git";
          rev = "d9d6f0f1f967807ffade6598ea9a631ebaf37a56";
        };

      in
      {
        devShells.default = pkgs.mkShell rec {
          nativeBuildInputs = [
            pkgs.pkg-config
            pkgs.gcc13
          ];

          buildInputs = with pkgs; [
            gdb
            nasm
            xorriso
            gcc
            gnumake
            bear
            nasm
            cmake

            wayland
            vulkan-loader
            vulkan-tools
            wayland
            wayland-protocols
            libXcursor
            libXrandr
            libXi
            vulkan-loader
            libxkbcommon
            stdenv.cc.cc.lib
            curl
            pkgs.glfw

          ];

          IMGUI_DIR = imgui;
          GLFW_DIR = glfw;

          shellHook = ''
            export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${pkgs.lib.makeLibraryPath buildInputs}
            exec zsh
          '';
        };
      }
    );
}
