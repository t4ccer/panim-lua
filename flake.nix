{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs";
  };

  outputs = { self, nixpkgs }: {

    devShells.x86_64-linux.default =
      let
        pkgs = nixpkgs.legacyPackages.x86_64-linux;
        lib = nixpkgs.lib;
      in pkgs.mkShell {
        shellHook = ''
          export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${lib.makeLibraryPath [ pkgs.libGL pkgs.xorg.libX11 ]}
        '';
        nativeBuildInputs = [
          (pkgs.c3c.overrideAttrs (old: {
            version = "0.6.6";
            src = old.src.override {
              rev = "bf74ef0e5e4dbadc9977d447fbbc07e576a3b8e8";
              hash = "sha256-BpUlM+hqp6FE9hH3JnhxV6ysz8G+IuzK/0zASxB4R0E=";
            };
            doCheck = false;
            doInstallCheck = false;
          }))
        ];
      };
  };
}
