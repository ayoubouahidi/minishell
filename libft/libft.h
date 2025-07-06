/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elkharti <elkharti@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 13:56:25 by ayouahid          #+#    #+#             */
/*   Updated: 2025/07/06 19:02:44 by elkharti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>
# include <stdint.h>
# include <limits.h>
# include "../includes/minishell.h"

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

int			ft_toupper(int c);
int			ft_tolower(int c);
int			ft_isalpha(int a);
int			ft_isdigit(int a);
int			ft_isascii(int a);
int			ft_isalnum(int a);
int			ft_isprint(int a);
size_t		ft_strlen(const char *a);
void		ft_bzero(void *a, size_t b);
void		*ft_memcpy(void *dest, const void *src, size_t numByte);
void		*ft_memset(void *a, int b, size_t c);
void		*ft_memmove(void *dest, const void *src, size_t numByte);
int			ft_atoi(const char *str);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strchr(const char *str, int c);
char		*ft_strtrim(char const *s1, char const *set);
size_t		ft_strlcpy(char *dest, const char *src, size_t size);
size_t		ft_strlcat(char *dest, const char *src, size_t size);
char		*ft_strrchr(const char *str, int c);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
void		*ft_memchr(const void *s, int c, size_t size);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
char		*ft_strnstr(const char *str, const char *find, size_t n);
char		*ft_strdup(const char *s);
char		**ft_split(char const *s, char c);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void		ft_striteri(char *s, void (*f)(unsigned int, char*));
void		ft_putchar_fd(char c, int fd);
void		*ft_calloc(size_t nmemb, size_t size);
char		*ft_itoa(int n);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);
t_list		*ft_lstnew(void *content);
void		ft_lstadd_front(t_list **lst, t_list *new);
int			ft_lstsize(t_list *lst);
void		ft_lstadd_back(t_list **lst, t_list *new);
void		ft_lstdelone(t_list *lst, void (*del)(void*));
void		ft_lstclear(t_list **lst, void (*del)(void *));
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstlast(t_list *lst);
int			ft_strcmp(const char *s1, const char *s2);

#endif
