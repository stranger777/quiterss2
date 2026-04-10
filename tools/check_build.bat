@echo off
echo ============================================
echo Checking if push reached origin/master
echo ============================================
git log --oneline origin/master -3
echo.
echo Latest commit on origin/master:
for /f "tokens=1" %%a in ('git log --oneline origin/master -1') do set SHA=%%a
echo SHA: %SHA%
echo.
echo ============================================
echo Checking what commit AppVeyor is building
echo ============================================
curl -s "https://ci.appveyor.com/api/projects/stranger777/quiterss2" > %TEMP%\av.json 2>nul
python -c "import json; d=json.load(open(r'%TEMP%\av.json','r',encoding='utf-8')); b=d.get('build',{}); print('Build version:', b.get('version','?')); print('Commit SHA: ', b.get('commitId','?')); j=b.get('jobs',[{}]); print('Status:    ', j[0].get('status','?') if j else 'no jobs')"
echo.
echo ============================================
echo Comparing...
echo ============================================
for /f "tokens=1" %%a in ('git log --oneline origin/master -1') do set LOCAL_SHA=%%a
python -c "import json; d=json.load(open(r'%TEMP%\av.json','r',encoding='utf-8')); b=d.get('build',{}); av_sha=(b.get('commitId','?') or '')[:7]; local='%LOCAL_SHA%'[:7]; print('origin/master:', local); print('AppVeyor:     ', av_sha); print('MATCH: YES' if local==av_sha else 'MATCH: NO — AppVeyor has not picked up the latest commit yet')"
