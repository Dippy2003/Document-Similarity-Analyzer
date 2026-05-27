@echo off
REM Quick smoke test for cosine-only mode.
echo Running cosine-only similarity on short samples...
PlagiarismChecker.exe samples\short1.txt samples\short2.txt --cosine-only

