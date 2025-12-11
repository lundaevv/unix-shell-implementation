/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:56:17 by vlundaev          #+#    #+#             */
/*   Updated: 2025/12/11 19:55:39 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

/*
** Create a new token node
** 'value' is a string created with malloc (from ft_substr)
*/
t_token	*token_new(char *value, t_token_type type)
{
	t_token	*node;

	if (!value)
		return (NULL);
	node = (t_token *)malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->value = value;
	node->type = type;
	node->next = NULL;
	return (node);
}

/*
** Add a token at the end of the linked list
** list -> pointer to head pointer
** new_node -> the node we insert
*/
void	token_add_back(t_token **list, t_token *new_node)
{
	t_token	*current;

	if (!list || !new_node)
		return ;
	if (!*list)
	{
		*list = new_node;
		return ;
	}
	current = *list;
	while (current->next != NULL)
		current = current->next;
	current->next = new_node;
}

/*
** Free all tokens in the list
** list -> pointer to head pointer
*/
void	token_list_clear(t_token **list)
{
	t_token	*cur;
	t_token	*next;

	if (!list)
		return ;
	cur = *list;
	while (cur)
	{
		next = cur->next;
		free(cur->value);
		free(cur);
		cur = next;
	}
	*list = NULL;
}
