// signals
void setup_interactive_signals(void);
void setup_builtin_signals(void);
void setup_exec_signals(void);
void setup_child_signals(void);
void setup_heredoc_terminal(void);
void restore_terminal(void);
void setup_heredoc_signals(void);
void sig_ctrl_c_heredoc(int signal);

// グローバル変数は１つだけ
extern int g_last_exit_status;

// 不要になった宣言を削除
// extern volatile sig_atomic_t g_heredoc_interrupted;

 