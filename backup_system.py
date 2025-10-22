#!/usr/bin/env python3
"""
MiniGit Backup and Safety System
This script provides automatic backup and safety checks for the MiniGit project
"""

import os
import shutil
import json
import hashlib
import datetime
from pathlib import Path

class MiniGitBackupSystem:
    def __init__(self, project_dir="."):
        self.project_dir = Path(project_dir).resolve()
        self.backup_dir = self.project_dir / "backups"
        self.safety_log = self.project_dir / "safety_log.json"
        self.critical_files = [
            "include/", "src/", "main.cpp", "README.md", 
            "build.sh", "compile.bat", "makefile"
        ]
        
    def create_backup(self, reason="manual"):
        """Create a timestamped backup of critical files"""
        timestamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
        backup_name = f"backup_{timestamp}_{reason}"
        backup_path = self.backup_dir / backup_name
        
        try:
            backup_path.mkdir(parents=True, exist_ok=True)
            
            # Backup critical files
            for item in self.critical_files:
                src = self.project_dir / item
                dst = backup_path / item
                
                if src.is_file():
                    shutil.copy2(src, dst)
                elif src.is_dir():
                    shutil.copytree(src, dst, dirs_exist_ok=True)
            
            # Create backup manifest
            manifest = {
                "timestamp": timestamp,
                "reason": reason,
                "files": list(self.critical_files),
                "backup_path": str(backup_path)
            }
            
            with open(backup_path / "manifest.json", "w") as f:
                json.dump(manifest, f, indent=2)
            
            self.log_safety_event("backup_created", f"Backup created: {backup_name}")
            print(f"✅ Backup created: {backup_name}")
            return backup_path
            
        except Exception as e:
            self.log_safety_event("backup_failed", f"Backup failed: {str(e)}")
            print(f"❌ Backup failed: {str(e)}")
            return None
    
    def restore_from_backup(self, backup_name):
        """Restore files from a specific backup"""
        backup_path = self.backup_dir / backup_name
        
        if not backup_path.exists():
            print(f"❌ Backup not found: {backup_name}")
            return False
        
        try:
            # Read manifest
            manifest_file = backup_path / "manifest.json"
            if manifest_file.exists():
                with open(manifest_file, "r") as f:
                    manifest = json.load(f)
            
            # Restore files
            for item in self.critical_files:
                src = backup_path / item
                dst = self.project_dir / item
                
                if src.exists():
                    if src.is_file():
                        shutil.copy2(src, dst)
                    elif src.is_dir():
                        if dst.exists():
                            shutil.rmtree(dst)
                        shutil.copytree(src, dst)
            
            self.log_safety_event("restore_completed", f"Restored from backup: {backup_name}")
            print(f"✅ Restored from backup: {backup_name}")
            return True
            
        except Exception as e:
            self.log_safety_event("restore_failed", f"Restore failed: {str(e)}")
            print(f"❌ Restore failed: {str(e)}")
            return False
    
    def check_file_integrity(self):
        """Check if critical files exist and are not corrupted"""
        missing_files = []
        corrupted_files = []
        
        for item in self.critical_files:
            file_path = self.project_dir / item
            if not file_path.exists():
                missing_files.append(str(item))
            elif file_path.is_file() and file_path.stat().st_size == 0:
                corrupted_files.append(str(item))
        
        if missing_files or corrupted_files:
            self.log_safety_event("integrity_check_failed", {
                "missing": missing_files,
                "corrupted": corrupted_files
            })
            return False, missing_files, corrupted_files
        
        self.log_safety_event("integrity_check_passed", "All critical files intact")
        return True, [], []
    
    def log_safety_event(self, event_type, details):
        """Log safety events for tracking"""
        log_entry = {
            "timestamp": datetime.datetime.now().isoformat(),
            "event": event_type,
            "details": details
        }
        
        # Load existing log
        log_data = []
        if self.safety_log.exists():
            try:
                with open(self.safety_log, "r") as f:
                    log_data = json.load(f)
            except:
                log_data = []
        
        # Add new entry
        log_data.append(log_entry)
        
        # Keep only last 100 entries
        if len(log_data) > 100:
            log_data = log_data[-100:]
        
        # Save log
        with open(self.safety_log, "w") as f:
            json.dump(log_data, f, indent=2)
    
    def list_backups(self):
        """List all available backups"""
        if not self.backup_dir.exists():
            print("No backups found")
            return []
        
        backups = []
        for backup_path in self.backup_dir.iterdir():
            if backup_path.is_dir():
                manifest_file = backup_path / "manifest.json"
                if manifest_file.exists():
                    with open(manifest_file, "r") as f:
                        manifest = json.load(f)
                    backups.append({
                        "name": backup_path.name,
                        "timestamp": manifest.get("timestamp", "unknown"),
                        "reason": manifest.get("reason", "unknown")
                    })
        
        return sorted(backups, key=lambda x: x["timestamp"], reverse=True)
    
    def auto_backup_before_operation(self, operation):
        """Automatically create backup before risky operations"""
        return self.create_backup(f"before_{operation}")

def main():
    backup_system = MiniGitBackupSystem()
    
    print("MiniGit Backup and Safety System")
    print("=" * 40)
    
    # Check current integrity
    is_healthy, missing, corrupted = backup_system.check_file_integrity()
    
    if not is_healthy:
        print("⚠️  File integrity issues detected:")
        if missing:
            print(f"   Missing files: {', '.join(missing)}")
        if corrupted:
            print(f"   Corrupted files: {', '.join(corrupted)}")
        
        # Try to restore from latest backup
        backups = backup_system.list_backups()
        if backups:
            print(f"\n🔄 Attempting to restore from latest backup: {backups[0]['name']}")
            backup_system.restore_from_backup(backups[0]['name'])
        else:
            print("\n❌ No backups available for restoration")
    else:
        print("✅ All critical files are intact")
    
    # List available backups
    backups = backup_system.list_backups()
    if backups:
        print(f"\n📁 Available backups ({len(backups)}):")
        for backup in backups[:5]:  # Show last 5
            print(f"   {backup['name']} - {backup['timestamp']} ({backup['reason']})")
    
    # Create a new backup
    print(f"\n💾 Creating new backup...")
    backup_system.create_backup("safety_check")

if __name__ == "__main__":
    main()
