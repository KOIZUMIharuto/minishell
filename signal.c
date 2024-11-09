# include "minishell.h"


// グローバル変数 (シグナルの番号を示す)
volatile sig_atomic_t g_signal_received = 0;

// シグナルハンドラ関数
void signal_handler(int signum) {
    if (signum == SIGINT) {
        g_signal_received = signum; // シグナル番号を設定
        write(1, "$\n", 2); // 新しい行にプロンプトを再表示
    }
}