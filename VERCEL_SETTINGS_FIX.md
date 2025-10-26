# 🔧 URGENT FIX: Force npm Instead of pnpm on Vercel

## The Problem

Vercel is detecting `pnpm` and trying to use it, but pnpm has registry connection issues on Vercel's servers.

## ✅ SOLUTION: Override Build Settings in Vercel Dashboard

### Step 1: Go to Your Project Settings

1. Go to [vercel.com/dashboard](https://vercel.com/dashboard)
2. Click on your project: `Smart-Library-Management-and-Monitoring-System-using-ESP32`
3. Click **"Settings"** (top menu)
4. Click **"General"** (left sidebar)

### Step 2: Set Root Directory

Scroll to **"Root Directory"**:
- Click **"Edit"**
- Enter: `web-dashboard`
- Click **"Save"**

### Step 3: Override Build Settings

Scroll to **"Build & Development Settings"**:

Click **"Override"** and set these values:

```
Framework Preset: Next.js

Build Command:
npm install && npm run build

Output Directory:
.next

Install Command:
npm install

Development Command:
npm run dev
```

**Important**: Make sure to use `npm` not `pnpm` in all commands!

### Step 4: Save Changes

Click **"Save"** at the bottom of the page.

### Step 5: Redeploy

1. Go to **"Deployments"** tab (top menu)
2. Find the latest deployment
3. Click the **"..."** menu (three dots)
4. Click **"Redeploy"**
5. Confirm by clicking **"Redeploy"** again

---

## Alternative: Use Vercel CLI

If the dashboard method doesn't work, deploy directly from CLI:

```bash
# Navigate to web-dashboard
cd web-dashboard

# Make sure no pnpm lock file exists
rm -f pnpm-lock.yaml

# Deploy with npm
vercel --prod
```

When asked about settings:
- Build Command: `npm run build`
- Output Directory: `.next`
- Development Command: `npm run dev`

---

## What I Just Did

I've added a `.npmrc` file to force npm usage:

```
# web-dashboard/.npmrc
package-manager=npm
```

This tells Vercel to use npm instead of pnpm.

**The changes have been pushed to GitHub!**

---

## ✅ Expected Result

After redeploying with these settings, you should see:

```
✓ Running "npm install"          ← Uses npm now!
✓ Installing dependencies
✓ Building application
✓ Build completed
✓ Deployment ready
```

No more pnpm errors!

---

## 🎯 Quick Checklist

- [ ] Root Directory set to `web-dashboard`
- [ ] Build Command set to `npm install && npm run build`
- [ ] Install Command set to `npm install`
- [ ] Override toggle is ON
- [ ] Redeployed after saving settings
- [ ] `.npmrc` file exists (already added)

---

## 📊 Verify Settings

Go to Settings → General → Build & Development Settings

Should look like this:

```
┌─────────────────────────────────────┐
│ Build & Development Settings        │
├─────────────────────────────────────┤
│ Override: [✓] ON                    │
│                                     │
│ Build Command:                      │
│ npm install && npm run build        │
│                                     │
│ Output Directory:                   │
│ .next                               │
│                                     │
│ Install Command:                    │
│ npm install                         │
│                                     │
│ Development Command:                │
│ npm run dev                         │
└─────────────────────────────────────┘
```

---

## 🆘 If Still Failing

1. **Check deployment logs** for the exact error
2. **Delete the project** from Vercel completely
3. **Re-import** from GitHub
4. **During import setup**:
   - Set Root Directory: `web-dashboard`
   - Override Build Command: `npm install && npm run build`
   - Add all 7 environment variables
5. Deploy

---

## 💡 Why This Happened

Vercel auto-detects package managers by looking for lock files:
- `pnpm-lock.yaml` → Uses pnpm
- `package-lock.json` → Uses npm
- `yarn.lock` → Uses yarn

Since you had `pnpm-lock.yaml`, it tried to use pnpm.

**Solution**: Remove pnpm lock files and force npm!

---

## ✅ Status

- ✅ Added `.npmrc` to force npm
- ✅ Removed `pnpm-lock.yaml`
- ✅ Pushed changes to GitHub
- ⏳ **YOU NEED TO**: Update build settings in Vercel Dashboard
- ⏳ **YOU NEED TO**: Redeploy

---

Follow the steps above and your deployment should work! 🚀
