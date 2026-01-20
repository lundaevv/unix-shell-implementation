#include "minishell.h"

static void	ms_set_ctx(void **ctx, char **envp, int *st)
{
	ctx[0] = envp;
	ctx[1] = st;
}

char	*ms_expand_unquote(const char *src, char **envp, int last_status)
{
	char	*dst;
	size_t	len;
	void	*ctx[2];
	int		j;

	len = ms_expanded_len(src, envp, last_status);
	dst = (char *)malloc(len + 1);
	if (!dst)
		return (NULL);
	ms_set_ctx(ctx, envp, &last_status);
	j = ms_expand_run(dst, src, ctx);
	dst[j] = '\0';
	return (dst);
}
