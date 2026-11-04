((nil . ((eglot-workspace-configuration
	  . (:luau-lsp (:platform (:type "standard"))))))
 ;; luau-lsp resolves relative definition paths against its own working
 ;; directory, so pass an absolute one
 (lua-mode . ((eval . (setq-local eglot-luau-custom-type-files
				  (list (concat "@voidEngine="
						(expand-file-name
						 "assets/scripts/types/voidengine.d.luau"
						 (locate-dominating-file default-directory ".dir-locals.el")))))))))
