#include <immintrin.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int main() {
	
	auto res = _xtest();

	if (res) {
		printf("Support\n");
	} else {
		printf("Not support\n");
	}

	int *g = (int *)malloc(sizeof(int));
	
	while (1) { // keep trying
		int status = _xbegin(); // set status = -1 and start transaction
		if (status == _XBEGIN_STARTED) { // status == XBEGIN_STARTED == -1
			(*g) ++; // non atomic increment of shared global variable
			_xend(); // end transaction
			break; // break on success
		} else { //
	
		} //
	}
}
