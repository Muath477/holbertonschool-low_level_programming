
/**
 * alloc_grid - returns a pointer to a 2 dimensional array of integers
 * @width: The width of the grid
 * @height: The height of the grid
 *
 * Description: Each element is initialized to 0. If any allocation
 * fails, everything already allocated is freed before returning NULL.
 *
 * Return: A pointer to the grid, or NULL on failure
 */
int **alloc_grid(int width, int height)
{
        int **grid;
        int i;
        int j;

        if (width <= 0 || height <= 0)
                return (NULL);

        grid = malloc(sizeof(int *) * height);
        if (grid == NULL)
                return (NULL);

        i = 0;
        while (i < height)
        {
                grid[i] = malloc(sizeof(int) * width);
                if (grid[i] == NULL)
                {
                        j = 0;
                        while (j < i)
                        {
                                free(grid[j]);
                                j+  
