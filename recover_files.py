#!/usr/bin/env python3
"""
MiniGit File Recovery Script
This script helps recover deleted or corrupted files
"""

import os
import shutil
import json
import glob
from pathlib import Path
from datetime import datetime

class FileRecovery:
    def __init__(self, project_dir="."):
        self.project_dir = Path(project_dir).resolve()
        self.backup_dir = self.project_dir / "backups"
        self.recovery_log = self.project_dir / "recovery_log.txt"
        
        # Define critical files that should exist
        self.critical_files = {
            "include/": "Directory with header files",
            "src/": "Directory with source files", 
            "main.cpp": "Main program file",
            "README.md": "Project documentation",
            "build.sh": "Unix build script",
            "compile.bat": "Windows build script",
            "makefile": "Make configuration"
        }
    
    def scan_for_missing_files(self):
        """Scan for missing critical files"""
        missing = []
        present = []
        
        print("🔍 Scanning for missing files...")
        print("=" * 50)
        
        for file_path, description in self.critical_files.items():
            full_path = self.project_dir / file_path
            if full_path.exists():
                present.append((file_path, description, "✅ Present"))
            else:
                missing.append((file_path, description, "❌ Missing"))
        
        # Display results
        for file_path, description, status in present + missing:
            print(f"{status} {file_path:<15} - {description}")
        
        return missing
    
    def find_recovery_sources(self):
        """Find potential sources for file recovery"""
        sources = []
        
        # Check backups
        if self.backup_dir.exists():
            for backup_path in self.backup_dir.iterdir():
                if backup_path.is_dir():
                    sources.append(("backup", backup_path, "Backup directory"))
        
        # Check if files exist in src/ directory
        src_dir = self.project_dir / "src"
        if src_dir.exists():
            for file_path in self.critical_files.keys():
                if file_path.endswith(".cpp") or file_path.endswith(".hpp"):
                    src_file = src_dir / file_path
                    if src_file.exists():
                        sources.append(("src", src_file, "Source directory"))
        
        # Check for any .cpp or .hpp files in the project
        for pattern in ["**/*.cpp", "**/*.hpp", "**/*.h"]:
            for file_path in self.project_dir.glob(pattern):
                if file_path.is_file():
                    sources.append(("project", file_path, "Project file"))
        
        return sources
    
    def recover_from_backup(self, backup_path, target_files=None):
        """Recover files from a specific backup"""
        if target_files is None:
            target_files = list(self.critical_files.keys())
        
        recovered = []
        failed = []
        
        print(f"🔄 Recovering from backup: {backup_path.name}")
        
        for file_name in target_files:
            src = backup_path / file_name
            dst = self.project_dir / file_name
            
            try:
                if src.exists():
                    # Create parent directory if needed
                    dst.parent.mkdir(parents=True, exist_ok=True)
                    
                    if src.is_file():
                        shutil.copy2(src, dst)
                    elif src.is_dir():
                        if dst.exists():
                            shutil.rmtree(dst)
                        shutil.copytree(src, dst)
                    
                    recovered.append(file_name)
                    print(f"  ✅ Recovered: {file_name}")
                else:
                    failed.append(file_name)
                    print(f"  ❌ Not found in backup: {file_name}")
            except Exception as e:
                failed.append(file_name)
                print(f"  ❌ Failed to recover {file_name}: {str(e)}")
        
        return recovered, failed
    
    def recover_from_sources(self, sources, target_files=None):
        """Recover files from various sources"""
        if target_files is None:
            target_files = list(self.critical_files.keys())
        
        recovered = []
        failed = []
        
        for file_name in target_files:
            if (self.project_dir / file_name).exists():
                continue  # File already exists
            
            found = False
            for source_type, source_path, description in sources:
                if source_type == "backup":
                    src_file = source_path / file_name
                    if src_file.exists():
                        try:
                            dst = self.project_dir / file_name
                            dst.parent.mkdir(parents=True, exist_ok=True)
                            
                            if src_file.is_file():
                                shutil.copy2(src_file, dst)
                            elif src_file.is_dir():
                                shutil.copytree(src_file, dst)
                            
                            recovered.append(file_name)
                            print(f"✅ Recovered {file_name} from {description}")
                            found = True
                            break
                        except Exception as e:
                            print(f"❌ Failed to recover {file_name} from {description}: {str(e)}")
                            continue
            
            if not found:
                failed.append(file_name)
                print(f"❌ Could not recover: {file_name}")
        
        return recovered, failed
    
    def create_emergency_backup(self):
        """Create an emergency backup of current state"""
        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        backup_name = f"emergency_backup_{timestamp}"
        backup_path = self.backup_dir / backup_name
        
        try:
            backup_path.mkdir(parents=True, exist_ok=True)
            
            # Backup whatever files exist
            for file_name in self.critical_files.keys():
                src = self.project_dir / file_name
                if src.exists():
                    dst = backup_path / file_name
                    if src.is_file():
                        shutil.copy2(src, dst)
                    elif src.is_dir():
                        shutil.copytree(src, dst)
            
            print(f"✅ Emergency backup created: {backup_name}")
            return backup_path
        except Exception as e:
            print(f"❌ Failed to create emergency backup: {str(e)}")
            return None
    
    def log_recovery(self, action, details):
        """Log recovery actions"""
        timestamp = datetime.now().isoformat()
        log_entry = f"[{timestamp}] {action}: {details}\n"
        
        with open(self.recovery_log, "a") as f:
            f.write(log_entry)
    
    def interactive_recovery(self):
        """Interactive recovery process"""
        print("🚨 MiniGit File Recovery Tool")
        print("=" * 50)
        
        # Scan for missing files
        missing = self.scan_for_missing_files()
        
        if not missing:
            print("\n✅ All critical files are present!")
            return
        
        print(f"\n❌ Found {len(missing)} missing files")
        
        # Find recovery sources
        sources = self.find_recovery_sources()
        if not sources:
            print("\n❌ No recovery sources found!")
            print("💡 Try running the backup system first: python backup_system.py")
            return
        
        print(f"\n🔍 Found {len(sources)} potential recovery sources:")
        for i, (source_type, source_path, description) in enumerate(sources):
            print(f"  {i+1}. {description}: {source_path.name}")
        
        # Create emergency backup first
        print("\n🛡️  Creating emergency backup of current state...")
        self.create_emergency_backup()
        
        # Attempt recovery
        print("\n🔄 Attempting automatic recovery...")
        missing_files = [item[0] for item in missing]
        recovered, failed = self.recover_from_sources(sources, missing_files)
        
        # Report results
        print(f"\n📊 Recovery Results:")
        print(f"  ✅ Recovered: {len(recovered)} files")
        print(f"  ❌ Failed: {len(failed)} files")
        
        if recovered:
            print(f"\n✅ Successfully recovered: {', '.join(recovered)}")
        
        if failed:
            print(f"\n❌ Could not recover: {', '.join(failed)}")
            print("\n💡 Manual recovery options:")
            print("  1. Check if files exist in other locations")
            print("  2. Restore from external backup")
            print("  3. Recreate files from scratch")
        
        # Log the recovery
        self.log_recovery("interactive_recovery", f"Recovered: {recovered}, Failed: {failed}")

def main():
    recovery = FileRecovery()
    recovery.interactive_recovery()

if __name__ == "__main__":
    main()
