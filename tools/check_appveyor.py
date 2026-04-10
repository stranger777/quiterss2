import urllib.request
import json
import time
import sys

OWNER = "stranger777"
REPO = "quiterss2"

# Get latest commit SHA from GitHub
url = f"https://api.github.com/repos/{OWNER}/{REPO}/commits?per_page=1"
req = urllib.request.Request(url, headers={"User-Agent": "Mozilla/5.0"})
with urllib.request.urlopen(req, timeout=10) as resp:
    commits = json.loads(resp.read().decode())
    latest_sha = commits[0]["sha"]
    print(f"Latest GitHub commit: {latest_sha[:7]}")

# Poll AppVeyor API
for attempt in range(1, 21):
    time.sleep(10)
    api_url = f"https://ci.appveyor.com/api/projects/{OWNER}/{REPO}"
    try:
        with urllib.request.urlopen(api_url, timeout=10) as resp:
            data = json.loads(resp.read().decode())
        build = data.get("build", {})
        jobs = build.get("jobs", [])
        if not jobs:
            print(f"  [{attempt}/20] No jobs yet, waiting...")
            continue
        job = jobs[0]
        commit = build.get("commitId", "")[:7]
        status = job.get("status", "unknown")
        version = build.get("version", "?")
        started = job.get("started", "?")

        print(f"  [{attempt}/20] Build #{version} | commit {commit} | status: {status} | started: {started}")

        if commit == latest_sha[:7]:
            print(f"\n✅ AppVeyor picked up commit {latest_sha[:7]}!")
            print(f"   Build #{version} is {status}")
            sys.exit(0)
        else:
            print(f"   ❌ Still on old commit {commit}, expected {latest_sha[:7]}")
    except Exception as e:
        print(f"  [{attempt}/20] Error: {e}")

print("\n❌ AppVeyor did not pick up the latest commit within 3.5 minutes")
sys.exit(1)
