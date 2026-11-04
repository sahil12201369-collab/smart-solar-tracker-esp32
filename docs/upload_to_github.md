# Uploading this project to GitHub

## Before you upload

1. Add your real photos to `images/` (see `images/README.md`) and put them in the README if you want.
2. **Never upload your Wi-Fi password.** Your real details live in `firmware/SolarTracker/config.h`, which is already in `.gitignore`. Check that `config.h` does **not** show in the upload list.
3. Edit the name and links at the bottom of `README.md`.

## Option A: GitHub website (no Git needed)

1. On github.com click **New repository**, name it `smart-solar-tracker-esp32`, choose Public, and create it.
2. Click **Add file → Upload files**, drag in the **contents** of the extracted folder (not the zip file itself), and click **Commit changes**.
   The website upload does not read `.gitignore`, so do not drag in your private `config.h`.

## Option B: Git command line

```bash
cd smart-solar-tracker-esp32
git init
git add .
git commit -m "Initial commit: smart solar tracker (ESP32, LDR, servo, dashboard)"
git branch -M main
git remote add origin https://github.com/<your-username>/smart-solar-tracker-esp32.git
git push -u origin main
```

## Show the dashboard without hardware (GitHub Pages)

`docs/demo.html` is the same dashboard page with simulated data, so anyone can see it working.

1. Open the repository **Settings → Pages**.
2. Under *Build and deployment*, choose **Deploy from a branch**, branch `main`, folder `/docs`, and save.
3. After a minute the demo is at `https://<your-username>.github.io/smart-solar-tracker-esp32/demo.html`. Put this link in the README and in your report.

## Link it in the report

Paste the repository link into the *GitHub Repository Link* page of the Word report (`report/`).
