{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = [
    pkgs.esphome

    # keep this line if you use bash
    pkgs.bashInteractive
  ];
}
