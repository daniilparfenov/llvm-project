// Function to avoid loop removing
extern void side_effect();

void nested_loops(int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            side_effect();
        }
    }
}
