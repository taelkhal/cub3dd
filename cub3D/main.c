/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amakhrou <amakhrou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 20:44:39 by amakhrou          #+#    #+#             */
/*   Updated: 2023/12/18 20:44:39 by amakhrou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../raycasting/cub3D.h"

float degtorad(float angle)
{
    return (angle * (pi / 180));
}

void	mlx_clear_image(t_game *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < data->map->width * 64)
    {
        j = 0;
        while (j < data->map->height * 64)
        {
            my_mlx_pixel_put(data, i, j, 0);
            j++;
        }
        i++;
    }
}

int key_hook(int key, t_game *game)
{
    float new_angle;
    int check;

    check = 0;
    if (key == 65307)
        exit(0);
    if (key == 65361)
    {
        game->player->angle -= 3;
        if (game->player->angle < 0)
            game->player->angle += 360.0;
        game->player->pdx = cos(degtorad(game->player->angle)) * 5;
        game->player->pdy = sin(degtorad(game->player->angle)) * 5;
    }
    if (key == 65363)
    {
        game->player->angle += 3;
        if (game->player->angle > 360.0)
            game->player->angle -= 360.0;
        game->player->pdx = cos(degtorad(game->player->angle)) * 5;
        game->player->pdy = sin(degtorad(game->player->angle)) * 5;
    }
    if (key == 97)// Key A (Move left)
    {
        new_angle = game->player->angle - 90.0;
        if (new_angle < 0)
            new_angle += 360.0;
        if (game->map->map[(int)(game->player->y + sin(degtorad(new_angle)) * 5) / game->size_px_map][(int)(game->player->x + cos(degtorad(new_angle)) * 5) / game->size_px_map] != '1')
        {
            game->player->x = game->player->x + cos(degtorad(new_angle)) * 5;
            game->player->y = game->player->y + sin(degtorad(new_angle)) * 5;
        }
    }
    if (key == 100)// Key D (Move right)
    {
        new_angle = game->player->angle + 90.0;
        if (new_angle > 360.0)
            new_angle -= 360.0;
        if (game->map->map[(int)(game->player->y + sin(degtorad(new_angle)) * 5) / game->size_px_map][(int)(game->player->x + cos(degtorad(new_angle)) * 5) / game->size_px_map] != '1')
        {
            game->player->x = game->player->x + cos(degtorad(new_angle)) * 5;
            game->player->y = game->player->y + sin(degtorad(new_angle)) * 5;
        }
    }
    if (key == 119)// Key W (Move forward)
    {
        if (check == 0)
        {
            game->player->pdx = cos(degtorad(game->player->angle)) * 5;
            game->player->pdy = sin(degtorad(game->player->angle)) * 5;
            check = 1;
        }
        if (game->map->map[(int)(game->player->y + game->player->pdy) / game->size_px_map][(int)(game->player->x + game->player->pdx) / game->size_px_map] != '1')
        {
            game->player->x = game->player->x + game->player->pdx;
            game->player->y = game->player->y + game->player->pdy;
        }
    }
    if (key == 115)// Key S (Move backward)
    {
        if (check == 0)
        {
            game->player->pdx = cos(degtorad(game->player->angle)) * 5;
            game->player->pdy = sin(degtorad(game->player->angle)) * 5;
            check = 1;
        }
        if (game->map->map[(int)(game->player->y - game->player->pdy) / game->size_px_map][(int)(game->player->x - game->player->pdx) / game->size_px_map] != '1')
        {
            game->player->x = game->player->x - game->player->pdx;
            game->player->y = game->player->y - game->player->pdy;
        }

    }
    int map_x = game->player->x / game->size_px_map;
    int map_y = game->player->y / game->size_px_map;

    if (map_x >= 0 && map_x < game->map->height && map_y >= 0 && map_y < game->map->width)
    {
        if (game->map->map[map_y][map_x] == '1')
        {
            game->player->x = game->player->x;
            game->player->y = game->player->y;
        }
    }
    return (0);
}
int    game_loop(t_game *game)
{
    mlx_destroy_image(game->mlx, game->img);
    game->img = mlx_new_image(game->mlx, WIN_W, WIN_H);
    game->addr = mlx_get_data_addr(game->img, &game->bits_per_pixel, &game->line_length, &game->endian);
    // draw_map(game);
    // mlx_clear_image(game);
    // draw_player(game, game->player->x, game->player->y);
    // cast_horizontal_rays(game, game->castrays);
    // cast_vertical_rays(game, game->castrays);
    // draw_thick_line(game, game->player->x, game->player->y, game->player->x + game->player->pdx * 5, game->player->y + game->player->pdy * 5, 0x0000FF);
    castoneray(game);
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
    return (0);
}

int main(int ac, char **av)
{
    (void)ac;
    t_map *map;
	t_parse *parse;
    t_game *game;
    t_castrays *castrays;

    game = (t_game *)malloc(sizeof(t_game));
    castrays = (t_castrays *)malloc(sizeof(t_castrays));
	parse = malloc(sizeof(t_parse));
    if (ac != 2)
        return (0);
    map = parser(av[1], parse);
	// printf ("%s\n", map->map[1]);
    init_game(game,map,castrays);
    get_angle(game);
    mlx_loop_hook(game->mlx, game_loop, game);
    mlx_hook(game->win, 2, 1L << 0, &key_hook, game);
    mlx_hook(game->win, 17, 0, NULL, 0);
    mlx_loop(game->mlx);

}