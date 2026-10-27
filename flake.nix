{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = {
    self,
    nixpkgs,
  }: let
    system = "x86_64-linux";
    pkgs = import nixpkgs {
      inherit system;
    };
  in {
    devShells.${system}.default = pkgs.mkShell.override {stdenv = pkgs.clangStdenv;} {
      packages = with pkgs; [
	lldb # clang debugger because why not
	cmake # spawn of satan
	ninja # google thing
	pkg-config # its obvious
	sdl3 # abstraction to handle user stuff
	sdl3-image # image
	luau # This doesn't even have a dev package, shit!
	python3 # for luau build
	spdlog # logging
	# upstream imgui package is outdated,
	# TODO bug maintainer to update or do it yourself
	# (imgui.override {
	#   IMGUI_BUILD_SDL3_BINDING = true;
	#   IMGUI_BUILD_SDL3_RENDERER_BINDING = true;
	# })
	alsa-lib
	dbus
	libGL
	libayatana-appindicator
	libdecor
	libdrm
	libjack2
	libpulseaudio
	libusb1
	libxkbcommon
	libgbm
	libx11
	libxcb
	libxscrnsaver
	libxcursor
	libxext
	libxfixes
	libxi
	libxrandr
	libxtst
	ninja
	pipewire
	sndio
	vulkan-headers
	vulkan-loader
	wayland
	wayland-scanner
	zenity
      ];
      CMAKE_GENERATOR = "Ninja";
    };
  };
}
