# ⚠️ IMPORTANT: Vercel Root Directory Configuration

## The Issue

Your Next.js app is located in the `web-dashboard` subdirectory, not the project root.

## Solution: Two Options

### Option 1: Deploy from web-dashboard folder directly (Easiest)

Navigate to the web-dashboard folder and deploy from there:

```bash
cd web-dashboard
vercel
```

When prompted:
- **Set up and deploy?** → `Y`
- **Which scope?** → Select your account
- **Project name?** → `smart-library-dashboard`
- **In which directory is your code located?** → `./` (leave as is)

This treats `web-dashboard` as the root, so Vercel will find everything correctly.

---

### Option 2: Deploy from project root with configuration

If deploying from the project root:

#### A. Via Vercel Dashboard (Recommended)

1. Go to [vercel.com](https://vercel.com)
2. Import your GitHub repository
3. **CRITICAL STEP**: In the import settings, find **"Root Directory"**
4. Click **"Edit"** next to Root Directory
5. Enter: `web-dashboard`
6. Click **"Continue"**
7. Add environment variables
8. Deploy

![Root Directory Setting](https://assets.vercel.com/image/upload/v1/root-directory.png)

#### B. Via Vercel CLI

From project root:

```bash
vercel --cwd web-dashboard
```

Or create a vercel configuration in the root.

---

## Verification

After deployment, check:

✅ Build succeeds without "No package.json found" error
✅ Dashboard loads at the Vercel URL
✅ All pages are accessible
✅ Firebase data displays correctly

---

## Quick Fix Commands

### If you get "No package.json found" error:

**Option 1**: Deploy from web-dashboard folder
```bash
cd web-dashboard
vercel --prod
```

**Option 2**: Use --cwd flag from root
```bash
vercel --cwd web-dashboard --prod
```

**Option 3**: Set root directory in Vercel dashboard
- Go to Project Settings
- Build & Development Settings
- Root Directory: `web-dashboard`
- Save and redeploy

---

## Environment Variables Location

Add environment variables in **Vercel Dashboard**:
1. Go to your project
2. Settings → Environment Variables
3. Add all `NEXT_PUBLIC_*` variables
4. Redeploy

---

## Recommended Approach

**Use Option 1** (deploy from web-dashboard folder):

```bash
# 1. Navigate to web-dashboard
cd web-dashboard

# 2. Deploy
vercel

# 3. Add environment variables
vercel env add NEXT_PUBLIC_FIREBASE_API_KEY
vercel env add NEXT_PUBLIC_FIREBASE_DATABASE_URL
# ... add all others

# 4. Deploy to production
vercel --prod
```

This is the cleanest and most straightforward approach!

---

## File Structure Reference

```
Smart Library Management System/
├── src/                          # ESP32 code
├── platformio.ini
├── web-dashboard/                ← Your Next.js app is HERE
│   ├── package.json             ← Vercel needs to find this
│   ├── app/
│   ├── components/
│   └── lib/
└── README.md
```

Make sure Vercel is looking in `web-dashboard` folder!
