;; eglot settings for luau-lsp
((nil . ((eglot-workspace-configuration
          . (:luau-lsp
             (:platform (:type "standard")
              :types (:definitionFiles
                      (:@voidEngine "assets/scripts/types/voidengine.d.luau"))))))))
