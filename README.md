# minishell

## Git 運用ルール

### ブランチ運用

1. **main**:
   - プロジェクト全体のコードを管理する安定したブランチ。安定した状態のコードのみをマージする。

2. **作業ブランチ**:
   - 新しい機能や修正は、`main` から派生したブランチで作業する。
   - **命名規則**: `担当者名/feature-作業内容` または `担当者名/fix-修正内容`
     - `担当者名`: 作業を行うメンバーの名前やイニシャルを使用。
     - `作業内容/修正内容`: 作業や修正の内容を簡潔に表現。
   - 例: `haruto/feature-add-prompt`, `shio/fix-memory-leak`

### コミットメッセージ

- コミットメッセージは簡潔に、どのような変更を行ったかを明確にする。
- 形式: `タグ: 簡単な説明`
  - 例: `feat: プロンプトの表示を追加`, `fix: メモリリークを修正`

### 作業の流れ

1. **作業ブランチの作成**:
   - `main` から新しいブランチを作成し、命名規則に従ってブランチ名を設定する。

2. **コミット**:
   - 作業が完了したら、こまめにコミットして進捗を管理する。

3. **プッシュ**:
   - 作業が一区切りついたら、リモートリポジトリにブランチをプッシュする。

4. **プルリクエストの作成**:
   - 作業ブランチを `main` にマージするためにプルリクエストを作成する。
   - プルリクエストには、変更内容の概要を簡潔に説明する。

5. **確認とマージ**:
   - もう一方のメンバーが内容を確認し、問題がなければ `main` にマージする。

### コーディングスタイル

- お互いに読みやすいコードを心がける。コメントや命名規則を統一し、全体で理解しやすいコードを目指す。
