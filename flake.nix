{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-25.05";
  };

  outputs = { self, nixpkgs }:
    let
      supportedSystems = [ "x86_64-linux" "aarch64-darwin" ];
      forEachSystem = nixpkgs.lib.genAttrs supportedSystems;
    in {
      devShells = forEachSystem (system: {
        default =
          let pkgs = nixpkgs.legacyPackages.${system}; in
          pkgs.mkShell {
            buildInputs = [
              pkgs.esphome

              # keep this line if you use bash
              pkgs.bashInteractive
            ];
          };
        });
    };
}
