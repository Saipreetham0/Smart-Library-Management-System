# 🚀 Vercel Deployment Guide

## Critical: You MUST Configure Root Directory in Vercel Settings

The deployment error occurs because Vercel is looking in the wrong directory. Follow these exact steps:

---

## Step-by-Step Deployment

### 1. Go to Vercel Dashboard
- Visit: https://vercel.com/dashboard
- Find your **Smart-Library-Management-System** project
- Click on it to open

### 2. Go to Settings
- Click the **Settings** tab at the top

### 3. Configure Root Directory (MOST IMPORTANT!)
- Scroll down to find **Root Directory** section
- Click **Edit** button
- Type: `web-dashboard`
- Click **Save**

**This is the critical step!** Without this, Vercel looks in the repository root where there's no package.json for the web app.

### 4. Verify Build & Development Settings
Should auto-detect once Root Directory is set:
- **Framework Preset**: Next.js
- **Build Command**: `npm run build`
- **Output Directory**: `.next`
- **Install Command**: `npm install`
- **Development Command**: `npm run dev`

### 5. Add Environment Variables
- Still in **Settings**, go to **Environment Variables**
- Add these variables (get values from Firebase Console):

```
NEXT_PUBLIC_FIREBASE_API_KEY=your-api-key
NEXT_PUBLIC_FIREBASE_AUTH_DOMAIN=your-project.firebaseapp.com
NEXT_PUBLIC_FIREBASE_DATABASE_URL=https://your-project.firebaseio.com
NEXT_PUBLIC_FIREBASE_PROJECT_ID=your-project-id
NEXT_PUBLIC_FIREBASE_STORAGE_BUCKET=your-project.appspot.com
NEXT_PUBLIC_FIREBASE_MESSAGING_SENDER_ID=your-sender-id
NEXT_PUBLIC_FIREBASE_APP_ID=your-app-id
```

- Set these for: **Production**, **Preview**, and **Development**

### 6. Trigger Deployment
Option A: **Redeploy from Deployments tab**
- Go to **Deployments** tab
- Click the latest failed deployment
- Click **Redeploy** button

Option B: **Push a new commit**
- Any new commit will trigger automatic deployment

---

## Troubleshooting

### Still getting "No Next.js version detected"?
✅ **Double-check Root Directory is set to `web-dashboard`**

This is in:
`Project Settings → General → Root Directory`

### How to verify Root Directory is set correctly?
After deployment starts, check the build logs. You should see:
```
Cloning completed
Running "vercel build"
```

And it should find your package.json with Next.js.

### Firebase not connecting?
Make sure all environment variables are added in Vercel dashboard.

---

## Expected Build Output

Once Root Directory is configured correctly, you should see:

```
✓ Cloning github.com/your-repo (Branch: main)
✓ Running "vercel build"
✓ Installing dependencies...
✓ Building Next.js application
✓ Build completed
✓ Deployment ready
```

---

## Quick Reference

**Repository Structure:**
```
repository-root/
├── src/              # ESP32 code
├── web-dashboard/    # ← Your Next.js app is HERE
│   ├── package.json  # ← Vercel needs to find THIS
│   ├── app/
│   ├── components/
│   └── ...
└── README.md
```

**Vercel needs to know:** "Look in `web-dashboard/` folder, not the root!"

**That's why Root Directory MUST be set to:** `web-dashboard`

---

## After Successful Deployment

1. Visit your deployment URL (vercel will provide this)
2. Check that the dashboard loads
3. Verify Firebase connection works
4. Test all features

---

**Need more help?** Check [web-dashboard/README.md](web-dashboard/README.md) for detailed web app documentation.
