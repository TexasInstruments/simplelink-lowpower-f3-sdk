# Board File Markdown Documentation

Board.md files are generated by [build_docs.py][build_docs_py]
based on [config.json](./config.json) and the `.md.j2` Jinja2 template files in this folder.

* `config.json`: Configuration file containing metadata for all boards.
* `*.md.j2`: Jinja2 template files

## Introduction

File `config.json` contains the board metadata required to create the Board.md files.

Each board has an entry in `config.json`. The board entry for
`source/ti/boards/CC1312R1_LAUNCHXL/docs/Board.md` is shown below.

The `key`("CC1312R1_LAUNCHXL" in the example below) represents the folder name
used for the board, e.g. `source/ti/boards/CC1312R1_LAUNCHXL/`.

```json
    "CC1312R1_LAUNCHXL": {
        "boardName": "CC1312R1_LAUNCHXL",
        "boardLongName": "SimpleLink&trade; CC1312R1 LaunchPad&trade;",
        "boardLink": "https://www.ti.com/tool/LAUNCHXL-CC1312R1",
        "deviceName": "CC1312R1F3",
        "deviceLink": "https://www.ti.com/product/CC1312R",
        "sections": {
            "settings_and_resources": "",
            "jumper_settings": [
                "leds",
                "uart_txd_rxd"
            ],
            "sysconfig_board_file": "",
            "driver_examples_resources": "",
            "booster_packs": {
                "boostxl_sharp128": "",
                "bp_bassensorsmkii": "",
                "cc3200audboost": "mod_din"
            }
        }
    },
```

* `boardName`: Board name (the same as the board `key`)
* `boardLongName`: The full name of the board, used in the title header
* `boardLink`: URL to the board on TI.com
* `deviceName`: Device specifier, e.g. "CC1312R1F3
* `deviceLink`: URL to the device on TI.com
* `sections`: Dict/object contaning `key`:`value` pairs of the Board.md sections
  * `key`: Name of the `*.md.j2` template file (excluding file extension)
  * `value`: Options passed to the template. This can be a String, list or dict (depends on what the template supports)
    * String: Template supports 1 (or 0) variant choices. Use empty string (`""`) for default.

      ```json
      "settings_and_resources": ""
      ```

    * List: Template supports selecting multiple variants.

      ```json
      "jumper_settings": ["leds", "uart_txd_rxd"]
      ```

    * Dict: Template supports `key`:`value` pairs of the Board.md sections (see `sections`)

      ```json
      "booster_packs": {
          "boostxl_sharp128": "",
          "cc3200audboost": "mod_din"
      }
      ```

## Generate Board.md files

To generate Board.md files, run [build_docs.py][build_docs_py] from Docker.
This generates Board.md files in `source/ti/boards/<board>/docs/`.

## Add a new board

To add a new board, open `config.json` and:

1. Copy/paste the board entry of a similar board.
   Please keep the board entry keys sorted alphabetically.
2. Update the new board entry.
3. Run [build_docs.py][build_docs_py] to see the resulting Board.md file in
   `source/ti/boards/<board>/docs/`.
4. Verify that the Board.md file is properly linted, using e.g. MS Code extension `DavidAnson.vscode-markdownlint`).

[build_docs_py]: ../../../../build/scripts/build_docs.py