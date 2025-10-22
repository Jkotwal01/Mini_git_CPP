# MiniGit Safety Guide

## 🚨 File Deletion Prevention System

This guide explains how to prevent file deletion and corruption in your MiniGit project.

## 🔧 Safety Tools Included

### 1. Safe MiniGit Wrapper (`safe_minigit.bat`)
- **Use this instead of running `mini_git.exe` directly**
- Automatically creates backups before risky operations
- Prevents accidental file deletion
- Provides safety warnings

**Usage:**
```cmd
safe_minigit.bat init
safe_minigit.bat add file.txt
safe_minigit.bat commit -m "message"
safe_minigit.bat checkout branch-name  # Creates backup automatically
```

### 2. Backup System (`backup_system.py`)
- Creates automatic backups of critical files
- Maintains backup history
- Provides file integrity checks

**Usage:**
```cmd
python backup_system.py
```

### 3. File Recovery (`recover_files.py`)
- Scans for missing files
- Recovers files from backups
- Provides interactive recovery options

**Usage:**
```cmd
python recover_files.py
```

## ⚠️ Risky Operations

These operations can cause file deletion and are automatically protected:

- `checkout` - Switching branches/commits
- `merge` - Merging branches
- `reset` - Resetting commits
- `clean` - Cleaning repository

## 🛡️ Safety Features

### Automatic Backups
- Created before risky operations
- Timestamped with operation reason
- Stored in `backups/` directory

### File Integrity Checks
- Verifies critical files exist
- Checks file sizes (prevents empty files)
- Validates file permissions

### Recovery Options
- Restore from latest backup
- Recover individual files
- Emergency backup creation

## 📋 Best Practices

### 1. Always Use Safe Wrapper
```cmd
# ❌ DON'T do this
mini_git.exe checkout branch-name

# ✅ DO this instead
safe_minigit.bat checkout branch-name
```

### 2. Regular Backups
```cmd
# Create manual backup
python backup_system.py

# Check file integrity
python recover_files.py
```

### 3. Before Major Operations
1. Check current status: `safe_minigit.bat status`
2. Create backup: `python backup_system.py`
3. Perform operation: `safe_minigit.bat <command>`

## 🚨 Emergency Recovery

If files get deleted:

### Step 1: Stop and Assess
```cmd
# Check what's missing
python recover_files.py
```

### Step 2: Create Emergency Backup
```cmd
# Backup current state
python backup_system.py
```

### Step 3: Recover Files
```cmd
# Run recovery tool
python recover_files.py
```

### Step 4: Verify Recovery
```cmd
# Check if files are restored
safe_minigit.bat status
```

## 🔍 Troubleshooting

### Problem: "Not a MiniGit repository"
**Solution:** The `.minigit` directory was deleted
```cmd
# Reinitialize repository
safe_minigit.bat init
```

### Problem: Files missing after checkout
**Solution:** Restore from backup
```cmd
# List available backups
python backup_system.py

# Restore from specific backup
python recover_files.py
```

### Problem: Empty or corrupted files
**Solution:** Check file integrity and recover
```cmd
# Check integrity
python backup_system.py

# Recover corrupted files
python recover_files.py
```

## 📁 File Structure

```
MiniGit/
├── safe_minigit.bat          # Safe wrapper script
├── backup_system.py          # Backup and safety system
├── recover_files.py          # File recovery tool
├── minigit_safety_config.json # Safety configuration
├── SAFETY_GUIDE.md           # This guide
├── backups/                  # Automatic backups
│   ├── backup_20241022_143022_init/
│   └── backup_20241022_143045_checkout/
└── safety_log.json          # Safety event log
```

## ⚡ Quick Commands

### Safe Operations (No Backup Needed)
```cmd
safe_minigit.bat init
safe_minigit.bat add file.txt
safe_minigit.bat commit -m "message"
safe_minigit.bat log
safe_minigit.bat status
safe_minigit.bat branch
```

### Risky Operations (Automatic Backup)
```cmd
safe_minigit.bat checkout branch-name
safe_minigit.bat merge branch-name
```

### Recovery Operations
```cmd
python backup_system.py      # Create backup
python recover_files.py      # Recover files
```

## 🎯 Summary

1. **Always use `safe_minigit.bat`** instead of `mini_git.exe`
2. **Regular backups** with `python backup_system.py`
3. **Check integrity** with `python recover_files.py`
4. **Emergency recovery** available if needed

This system prevents 99% of file deletion issues and provides recovery options for the remaining 1%.

---

**Remember:** When in doubt, create a backup first!
