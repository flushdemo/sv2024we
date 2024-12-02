        public _asset_palette
        public _asset_fond
        public _asset_fonte
        public _asset_musique

        section data

_asset_palette  = *
_asset_fond     = * + 32
_asset_fonte    = * + 32 + 32000
_asset_musique  = * + 32 + 32000 + 7680

        incbin "compact_assets.bin"
