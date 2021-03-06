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

// Compile with cc -lmlx -framework OpenGL -framework AppKit -framework OpenCL

#include <mlx.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <OpenCL/cl.h>

typedef struct	s_gpu{
	cl_int err;
	cl_platform_id ptm;
	cl_uint ptms;
	cl_device_id dvc;
	cl_uint dvcs;
}				t_gpu;

typedef struct  s_frcl{
    double  camx;
    double  camy;
    double  camz;
    int     winsx;
    int     winsy;
	int		iter;
}               t_frcl;

typedef struct	s_test{
	void	*mlx;
	void	*win;
	t_frcl	param;
}				t_test;

int		gpu_calcul(t_frcl param, void *mlx, void *win, int x,  int y);

int		key_press(int key, t_test *test)
{
	static int dx = 0;
	static int dy = 0;
	static int spc = 500;

	printf("key :%d\n", key);
	if (key == 53)
		exit(0);
	else if (key == 78)
		test->param.camz += 5 + test->param.camz / 50;
	else if (key == 69)
		test->param.camz -= 15 + test->param.camz / 50;
	else if (key == 123)
		test->param.camx -= 5 + test->param.camz / 50;
	else if (key == 124)
		test->param.camx += 5 + test->param.camz / 50;
	else if (key == 125)
		test->param.camy += 5 + test->param.camz / 50;
	else if (key == 126)
		test->param.camy -= 5 + test->param.camz / 50;
	gpu_calcul(test->param, test->mlx, test->win, test->param.camx, test->param.camy);
	return (0);
}

int		key_release(int key, t_test *test)
{
	printf("key release :%d\n", key);
	return (0);
}

int		mouse_hook(int button, int x, int y, t_test *test)
{
	if (button == 4)
		test->param.camz += 5;
	if (button == 5)
		test->param.camz -= 5;
	return (0);
}

int		mouse_move(int x, int y, t_test *test)
{
	return (0);
}

void fill_pixel(unsigned int *istr, int x, int y, int color)
{
	istr[x + y * 1000] = color;
}

const char color_cl[] = "\
\
typedef struct  s_frcl{\
    double  camx;\
    double  camy;\
    double  camz;\
    int     winsx;\
    int     winsy;\
	int		iter;\
}               t_frcl;\
\
__kernel void color(__global const t_frcl *in, __global unsigned int *out)\
{\
	const uint index = get_global_id(0);\
	const uint x = index % in->winsx;\
	const uint y = index / in->winsx;\
	const double pr = x / in->camz - (in->camx + in->winsx / 4)/ (in->camz * 2);\
	const double pi = y / in->camz - (in->camy + in->winsy / 4)/ (in->camz * 2);\
\
	double	new_r;\
	double	new_i;\
	double	old_r;\
	double	old_i;\
	int		i;\
\
	new_r = 0;\
	new_i = 0;\
	i = 0;\
	while ((new_r * new_r + new_i * new_i) < 4.0 && i < in->iter)\
	{\
		old_r = new_r;\
		old_i = new_i;\
		new_r = old_r * old_r - old_i * old_i + pr;\
		new_i = 2.0 * old_r * old_i + pi;\
		i++;\
	}\
	out[index] = i * 0x11FF11;\
}\
";


int		gpu_calcul(t_frcl param, void *mlx, void *win, int x,  int y)
{
	t_gpu tcl;
	int bpp = 32;
	int s_l = 1000 * 4;
	int endian = 0;

	param.camx = x;
	param.camy = y;

	void *img = mlx_new_image(mlx, param.winsx, param.winsy);
	unsigned int *istr = (unsigned int *)mlx_get_data_addr(img, &(bpp), &(s_l), &(endian));

	// Setup GPU
	// Fetch the Platform and Device IDs; we only want one.
	tcl.err = clGetPlatformIDs(1, &tcl.ptm, &tcl.ptms);
	printf("Err clGetPlatformIDs? :\t\t%d\n", tcl.err);
	tcl.err = clGetDeviceIDs(tcl.ptm, CL_DEVICE_TYPE_ALL, 1, &tcl.dvc, &tcl.dvcs);
	printf("Err clGetDeviceIDs? :\t\t%d\n", tcl.err);
	cl_context_properties properties[] = {
		CL_CONTEXT_PLATFORM,
		(cl_context_properties)tcl.ptm,
		0
	};
	// Note that nVidia's OpenCL requires the platform property
	cl_context context = clCreateContext(properties, 1, &tcl.dvc, NULL, NULL, &tcl.err);
	printf("Err clCreateContext? :\t\t%d\n", tcl.err);
	cl_command_queue cq = clCreateCommandQueue(context, tcl.dvc, 0, &tcl.err);
	printf("Err clCreateCommandQueue? :\t%d\n", tcl.err);

	const char *src = color_cl;
	size_t srcsize = strlen(src);

	// Submit the source code of the rot13 kernel to OpenCL
	cl_program prog = clCreateProgramWithSource(context, 1, &src, &srcsize, &tcl.err);
	printf("Err clCreateProgramWithSource? :%d\n", tcl.err);
	// and compile it (after this we could extract the compiled version)
	tcl.err = clBuildProgram(prog, 0, NULL, "", NULL, NULL);
	printf("Err clBuildProgram? :\t\t%d\n", tcl.err);

	// Allocate memory for the kernel to work with
	cl_mem mem1, mem2;
	// Worksize should be the size of your window (in pixel)
	size_t worksize = param.winsx * param.winsy;
	mem1 = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(param), NULL, &tcl.err);
	printf("Err clCreateBuffer? :\t\t%d\n", tcl.err);
	mem2 = clCreateBuffer(context, CL_MEM_USE_HOST_PTR, worksize, istr, &tcl.err);
	printf("Err clCreateBuffer? :\t\t%d\n", tcl.err);

	// get a handle and map parameters for the kernel
	cl_kernel k_color = clCreateKernel(prog, "color", &tcl.err);
	clSetKernelArg(k_color, 0, sizeof(mem1), &mem1);
	clSetKernelArg(k_color, 1, sizeof(mem2), &mem2);

	// Send input data to OpenCL (async, don't alter the buffer!)
	tcl.err = clEnqueueWriteBuffer(cq, mem1, CL_FALSE, 0, sizeof(param), &param, 0, NULL, NULL);
	printf("Err clEnqueueWriteBuffer? :\t%d\n", tcl.err);
	// Perform the operation
	tcl.err = clEnqueueNDRangeKernel(cq, k_color, 1, NULL, &worksize, NULL, 0, NULL, NULL);
	printf("Err clEnqueueNDRangeKernel? :\t%d\n", tcl.err);
	// Read the result back into buf2
	
	tcl.err = clEnqueueReadBuffer(cq, mem2, CL_FALSE, 0, worksize, istr, 0, NULL, NULL);
	printf("Err clEnqueueReadBuffer? :\t%d\n", tcl.err);
	// Await completion of all the above
	tcl.err = clFinish(cq);
	printf("Err clFinish? :\t\t\t%d\n", tcl.err);

	// End GPU

	mlx_put_image_to_window(istr, win, img, 0, 0);
	mlx_destroy_image(mlx, img);

	return (0);
}

int		main()
{
	t_gpu tcl;
	t_frcl param;
	t_test test;

	param.winsx = 1000;
	param.winsy = 1000;
	param.camx = 0;
	param.camy = 0;
	param.camz = 100;
	param.iter = 144;

	// Setup MLX
	void *mlx = mlx_init();
	void *win = mlx_new_window(mlx, param.winsx, param.winsy, "Test");

	test.mlx = mlx;
	test.win = win;
	test.param = param;
	gpu_calcul(test.param, test.mlx, test.win, 0, 0);

	mlx_hook(win, 2, 0, key_press, &test);
	mlx_hook(win, 3, 0, key_release, &test);
	mlx_hook(win, 6, 0, mouse_move, &test);
	mlx_mouse_hook(win, mouse_hook, &test);
	mlx_loop(mlx);
	return (0);
}
