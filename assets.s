        public _asset_palette
        public _asset_fond
        public _asset_fonte
        public _asset_musique

        section data

_asset_palette  = * + 2
_asset_fond     = * + 34
_asset_fonte    = * + 32034 + 34
_asset_musique  = * + 2*32034

        incbin "Assets/fond.pi1"
        incbin "Assets/fonte.pi1"
        incbin "Assets/FXMS2401.SND"
