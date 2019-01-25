/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testmlx.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magrab <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 22:09:05 by magrab            #+#    #+#             */
/*   Updated: 2019/01/25 16:38:48 by magrab           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct	s_value{
	int x;
	int y;
	int z;
	int c;
}				t_value;

t_value	**calcmap(int setup)
{
	static t_value **test;

	if (setup == 1)
	{
		test = malloc(5 * sizeof(t_value **));
		test[0] = malloc(3 * sizeof(t_value));
		test[0][0].x = 0;
		test[0][0].y = 0;
		test[0][0].z = 0;
		test[0][0].c = 0xFF0000;
		test[0][1].x = 0;
		test[0][1].y = 0;
		test[0][1].z = 10;
		test[0][1].c = 0xFF8000;
		test[0][2].c = -42;
		test[1] = malloc(3 * sizeof(t_value));
		test[1][0].x = 0;
		test[1][0].y = 10;
		test[1][0].z = 0;
		test[1][0].c = 0xFF0080;
		test[1][1].x = 0;
		test[1][1].y = 10;
		test[1][1].z = 10;
		test[1][1].c = 0xFFFFFF;
		test[1][2].c = -42;
		test[2] = malloc(3 * sizeof(t_value));
		test[2][0].x = 10;
		test[2][0].y = 0;
		test[2][0].z = 0;
		test[2][0].c = 0xFFFFFF;
		test[2][1].x = 10;
		test[2][1].y = 0;
		test[2][1].z = 10;
		test[2][1].c = 0xFFFFFF;
		test[2][2].c = -42;
		test[3] = malloc(3 * sizeof(t_value));
		test[3][0].x = 10;
		test[3][0].y = 10;
		test[3][0].z = 0;
		test[3][0].c = 0xFFFFFF;
		test[3][1].x = 10;
		test[3][1].y = 10;
		test[3][1].z = 10;
		test[3][1].c = 0xFFFFFF;
		test[3][2].c = -42;
		test[4] = NULL;
	}
	return (test);
}

int		key_press(int key, void *param)
{
	static int dx = 0;
	static int dy = 0;
	static int spc = 500;

	printf("key :%d\n", key);
	if (key == 53)
		exit(0);
	/*	if (key == 50)
		drawmap(calcmap(0), dx, dy, spc);
		if (key == 69)
		drawmap(calcmap(0), dx, dy, spc += 10);
		if (key == 78)
		drawmap(calcmap(0), dx, dy, spc -= 10);
		if (key == 123)
		drawmap(calcmap(0), dx -= 1, dy, spc);
		if (key == 124)
		drawmap(calcmap(0), dx += 1, dy, spc);
		if (key == 125)
		drawmap(calcmap(0), dx, dy += 1, spc);
		if (key == 126)
		drawmap(calcmap(0), dx, dy -= 1, spc);
		*/
	return (0);
}

int		key_release(int key, void *param)
{
	printf("key release :%d\n", key);
	return (0);
}

int		mouse_hook(int button, int x, int y, void *win)
{
	printf("button :%d\n", button);
	return (0);
}

void fill_pixel(unsigned int *istr, int x, int y, int color)
{
	istr[x + y * 1000] = color;
}

int		main()
{
	void *mlx = mlx_init();
	void *win = mlx_new_window(mlx, 1000, 1000, "Test");
	void *img = mlx_new_image(mlx, 1000, 1000);
	int bpp = 32;
	int s_l = 1000 * 4;
	int endian = 0;
	unsigned int *istr =  (unsigned int *)mlx_get_data_addr(img, &(bpp), &(s_l), &(endian));

	mlx_hook(win, 2, 0, key_press, win);
	mlx_hook(win, 3, 0, key_release, win);
	mlx_mouse_hook(win, mouse_hook, win);

	int x;
	x = 0xFF0001;
	while (--x > 0x10000)
	{
		fill_pixel(istr, x / 0x10000, 0, x);
		fill_pixel(istr, x / 0x10000, 1, x);
		fill_pixel(istr, x / 0x10000, 2, x);
		fill_pixel(istr, x / 0x10000, 3, x);
		fill_pixel(istr, x / 0x10000, 4, x);
		fill_pixel(istr, x / 0x10000, 5, x);
		fill_pixel(istr, x / 0x10000, 6, x);
		fill_pixel(istr, x / 0x10000, 7, x);
		fill_pixel(istr, x / 0x10000, 8, x);
		fill_pixel(istr, x / 0x10000, 9, x);
	}
	while (--x > 0x100)
	{
		fill_pixel(istr, x / 0x100, 10, x);
		fill_pixel(istr, x / 0x100, 11, x);
		fill_pixel(istr, x / 0x100, 12, x);
		fill_pixel(istr, x / 0x100, 13, x);
		fill_pixel(istr, x / 0x100, 14, x);
		fill_pixel(istr, x / 0x100, 15, x);
		fill_pixel(istr, x / 0x100, 16, x);
		fill_pixel(istr, x / 0x100, 17, x);
		fill_pixel(istr, x / 0x100, 18, x);
		fill_pixel(istr, x / 0x100, 19, x);
	}
	while (--x > 0x1)
	{
		fill_pixel(istr, x / 0x1, 20, x);
		fill_pixel(istr, x / 0x1, 21, x);
		fill_pixel(istr, x / 0x1, 22, x);
		fill_pixel(istr, x / 0x1, 23, x);
		fill_pixel(istr, x / 0x1, 24, x);
		fill_pixel(istr, x / 0x1, 25, x);
		fill_pixel(istr, x / 0x1, 26, x);
		fill_pixel(istr, x / 0x1, 27, x);
		fill_pixel(istr, x / 0x1, 28, x);
		fill_pixel(istr, x / 0x1, 29, x);
	}

	mlx_put_image_to_window(istr, win, img, 0, 0);
	mlx_loop(mlx);
	return (0);
}
