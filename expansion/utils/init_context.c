#include "../expansion.h"

void	init_expand_ctx(t_expand_ctx *ctx, const char *arg, t_env *env)
{
	ctx->arg = arg;
	ctx->env = env;
	ctx->i = 0;
	ctx->j = 0;
	ctx->in_squote = 0;
	ctx->in_dquote = 0;
}
