-- project-local config, needs `vim.o.exrc = true` (neovim 0.11+)
vim.lsp.config("luau_lsp", {
  settings = {
    ["luau-lsp"] = {
      platform = { type = "standard" },
      types = {
        definitionFiles = { ["@voidEngine"] = "assets/scripts/types/voidengine.d.luau" },
      },
    },
  },
})
