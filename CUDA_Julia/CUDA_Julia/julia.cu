#include "common/book.h"
#include "common/cpu_bitmap.h"

#define DIM 1000

__device__ int julia(int x, int y)
{
	const float scale = 1.5;
	float jx = scale * (float)(DIM/2 - x) / (DIM/2);
	float jy = scale * (float)(DIM/2 - y) / (DIM/2);

	float c_r = -0.8, c_i = 0.156;
	float a_r = jx, a_i = jy;

	for (int i = 0; i < 200; i++)
	{
		float next_a_r = a_r * a_r - a_i * a_i + c_r;
		float next_a_i = a_i * a_r + a_r * a_i + c_i;

		a_r = next_a_r; 
		a_i = next_a_i;

		if (a_r * a_r + a_i * a_i > 1000)
			return 0;
	}

	return 1;
}

__global__ void kernel(unsigned char *ptr)
{
	int x = blockIdx.x;
	int y = blockIdx.y;
	int offset = x + y * gridDim.x;

	int juliaValue = julia(x, y);
	ptr[offset * 4 + 0] = 255 * juliaValue;
	ptr[offset * 4 + 1] = 0;
	ptr[offset * 4 + 2] = 0;
	ptr[offset * 4 + 3] = 255;
}

int main() 
{
	CPUBitmap bitmap(DIM, DIM, NULL); 
	unsigned char *device_bitmap;

	HANDLE_ERROR( cudaMalloc((void**)&device_bitmap, bitmap.image_size()) );

	dim3 grid(DIM, DIM);
	kernel<<<grid, 1>>>(device_bitmap);

	HANDLE_ERROR( cudaMemcpy(bitmap.get_ptr(), device_bitmap, bitmap.image_size(), cudaMemcpyDeviceToHost) );

	bitmap.display_and_exit(NULL);

	HANDLE_ERROR( cudaFree(device_bitmap) );

	return 0;
}
