
/**
 * \brief Compare two integers
 *
 * A function to be used with qsort().
 *
 * \param[in] a First integer.
 * \param[in] b Second integer.
 *
 * \retval -1 a smaller than b.
 * \retval  0 a equals b.
 * \retval  1 a larger than b.
 */
int compare_ints(const void* a, const void* b);
