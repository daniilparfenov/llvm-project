// Function to avoid loop removing
extern void side_effect();

void simple_loop(int n) {
    for (int i = 0; i < n; i++) {
        side_effect();
    }
}
