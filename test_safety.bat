@echo off
echo Testing MiniGit Safety System
echo =============================

echo.
echo 1. Testing file integrity...
python backup_system.py

echo.
echo 2. Testing MiniGit initialization...
mini_git.exe init

echo.
echo 3. Testing safe wrapper...
echo init | safe_minigit.bat

echo.
echo 4. Testing recovery system...
python recover_files.py

echo.
echo 5. Creating test files...
echo "Test content" > test.txt
echo "Another test" > test2.txt

echo.
echo 6. Testing add operation...
mini_git.exe add test.txt test2.txt

echo.
echo 7. Testing commit...
mini_git.exe commit -m "Test commit"

echo.
echo 8. Testing status...
mini_git.exe status

echo.
echo 9. Testing log...
mini_git.exe log

echo.
echo 10. Testing branch creation...
mini_git.exe branch test-branch

echo.
echo 11. Testing branch listing...
mini_git.exe branch

echo.
echo 12. Testing safe checkout (with backup)...
echo test-branch | safe_minigit.bat checkout

echo.
echo 13. Final status check...
mini_git.exe status

echo.
echo =============================
echo Safety system test completed!
echo =============================
pause
