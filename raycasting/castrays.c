/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   castrays.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taelkhal <taelkhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 15:54:00 by taelkhal          #+#    #+#             */
/*   Updated: 2023/12/11 15:54:00 by taelkhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

float fixang(float ang)
{
    if (ang < 0)
        ang += 360.0;
    if (ang > 360.0)
        ang -= 360.0;
    return (ang);
}

void	ft_drawfloor(t_game *game, float lineh, float lineo)
{
	int		j;

	j = (int)(lineh + lineo);
	while (j < WIN_H)
	{
		my_mlx_pixel_put(game, game->i2, j, 0x00FF00);
		j++;
	}
}
void	ft_drawceilling(t_game *game, float lineo)
{
	int	i;

	i = 0;
	while (i < lineo)
	{
		my_mlx_pixel_put(game, game->i2, i, 2551233);
		i++;
	}
}
float	dist(float ax, float ay, float bx, float by)
{
	return (sqrt(((bx - ax) * (bx - ax)) + ((by - ay) * (by - ay))));
}

void convert_3d(t_game *game, int check)
{
    float lineo;
    float lenn;
    float wall_h = fixang(game->player->angle - game->castrays->ra); /* (WIN_W / 2) / (game->castrays->ra);*/
    // int wall_strip_h = (int)(wall_h);
    // int wall_top_px = (WIN_H / 2) - (wall_strip_h / 2);
    // int wall_bottom_px = (WIN_H / 2) + (wall_strip_h / 2);
    // int wall_strip_h = (int)(64 / wall_h * 277);
    // int wall_top_px = (WIN_H / 2) - (wall_strip_h / 2);
    // int wall_bottom_px = (WIN_H / 2) + (wall_strip_h / 2);
    float len = dist(game->player->x, game->player->y, game->castrays->rx, game->castrays->ry);
    len = len * cos(degtorad(wall_h));
    lenn = (64 * WIN_H) / len;
    int i = 0;
    if (lenn > WIN_H)
        lenn = WIN_H;
    lineo = (WIN_H / 2.0) - (lenn / 2.0);

    // castoneray(game);
    while (i < WIN_H)
    {
        if (i > lineo && i < (lineo + lenn))
        {
            if (check == 1)
                my_mlx_pixel_put(game, game->i2, i, 0x808080);
        }
        i++;
    }
    ft_drawfloor(game, lenn, lineo);
    ft_drawceilling(game, lineo);
}

void casting(t_game *game, float x, float y)
{
    while (1)
    {
        if (game->map->map[(int)(game->castrays->ry + y) / 64][(int)(game->castrays->rx + x) / 64] == '1')
        {
            convert_3d(game, 1);
            return;
        }
        game->castrays->rx += x;
        game->castrays->ry += y;
        // my_mlx_pixel_put(game, game->castrays->rx, game->castrays->ry, 0x00FF00);
    }
}
void castoneray(t_game *game)
{
    float x;
    float y;

    game->castrays->ra = fixang(game->player->angle - 30.0);
    game->i2 = 0;
    while (game->i2 < WIN_W)
    {
        game->castrays->rx = game->player->x;
        game->castrays->ry = game->player->y;
        x = cos(degtorad(game->castrays->ra));
        y = sin(degtorad(game->castrays->ra));
        casting(game, x, y);
        game->i2++;
        game->castrays->ra += 60.0 / (WIN_W);
        game->castrays->ra = fixang(game->castrays->ra);
    }
}