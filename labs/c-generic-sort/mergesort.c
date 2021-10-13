
void mergesort(void *lineptr[], int left, int right, int (*comp)(void *, void *))
{
    void merge(void *lineptr[], int, int, int, int (*comp)(void *, void *));
    if (left >= right)
        return;
    int mid = left + (right - left) / 2;
    mergesort(lineptr, left, mid, comp);
    mergesort(lineptr, mid + 1, right, comp);
    merge(lineptr, left, mid, right, comp);
}

void merge(void *lineptr[], int left, int mid, int right, int (*comp)(void *, void *))
{
    int lsize = mid - left + 1;
    int rsize = right - mid;
    void *larr[lsize];
	void *rarr[rsize];
    for (int i = 0; i < lsize; i++) {
        larr[i] = lineptr[left + i];
    }
    for (int j = 0; j < rsize; j++) {
        rarr[j] = lineptr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;
    while (i < lsize && j < rsize) {
        if ((*comp)(larr[i], rarr[j]) <= 0) {
            lineptr[k] = larr[i];
            i++;
        }
        else {
            lineptr[k] = rarr[j];
            j++;
        }
        k++;
    }
  
    while (i < lsize) {
        lineptr[k] = larr[i];
        i++;
        k++;
    }
  
    while (j < rsize) {
        lineptr[k] = rarr[j];
        j++;
        k++;
    }
}
