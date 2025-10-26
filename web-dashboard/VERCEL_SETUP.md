# ✅ Vercel Deployment - Root Directory Fix

## The Problem

When deploying to Vercel, you might see:

```
❌ Error: No package.json found
```

This happens because your Next.js app is in the `web-dashboard` subdirectory.

---

## ✅ The Solution: 2 Easy Ways

### Method 1: Deploy from web-dashboard folder (RECOMMENDED)

This is the **easiest and cleanest** method:

```bash
# Navigate to web-dashboard folder
cd web-dashboard

# Login to Vercel
vercel login

# Deploy
vercel

# Follow prompts, then deploy to production
vercel --prod
```

✅ **This works because you're deploying from the correct folder!**

---

### Method 2: Deploy via GitHub with Root Directory setting

If using GitHub integration:

1. **Push to GitHub**
   ```bash
   git add .
   git commit -m "Add web dashboard"
   git push
   ```

2. **Import to Vercel**
   - Go to [vercel.com/new](https://vercel.com/new)
   - Click **"Import Git Repository"**
   - Select your repository

3. **⚠️ CRITICAL STEP - Set Root Directory**

   In the import configuration screen:

   ```
   Framework Preset: Next.js (detected) ✓

   Root Directory: ./  ← CLICK "Edit" HERE!
                    ↓
   Root Directory: web-dashboard  ← SET THIS!

   Build Command: pnpm build (auto-detected)
   Output Directory: .next (auto-detected)
   Install Command: pnpm install (auto-detected)
   ```

4. **Add Environment Variables**

   Click "Environment Variables" and add:
   ```
   NEXT_PUBLIC_FIREBASE_API_KEY = your-value
   NEXT_PUBLIC_FIREBASE_DATABASE_URL = your-value
   NEXT_PUBLIC_FIREBASE_PROJECT_ID = your-value
   NEXT_PUBLIC_FIREBASE_AUTH_DOMAIN = your-value
   NEXT_PUBLIC_FIREBASE_STORAGE_BUCKET = your-value
   NEXT_PUBLIC_FIREBASE_MESSAGING_SENDER_ID = your-value
   NEXT_PUBLIC_FIREBASE_APP_ID = your-value
   ```

5. **Deploy**

   Click "Deploy" button and wait ~2 minutes.

---

## Visual Guide

### Where to find Root Directory setting:

```
┌─────────────────────────────────────────────┐
│  Configure Project                          │
├─────────────────────────────────────────────┤
│                                             │
│  Project Name                               │
│  ┌────────────────────────────────────┐    │
│  │ smart-library-dashboard            │    │
│  └────────────────────────────────────┘    │
│                                             │
│  Framework Preset: Next.js            ✓    │
│                                             │
│  Root Directory: ./         [Edit] ← CLICK  │
│  ┌────────────────────────────────────┐    │
│  │ web-dashboard                      │    │ ← TYPE THIS
│  └────────────────────────────────────┘    │
│                                             │
│  Build and Output Settings                  │
│  Build Command: pnpm build            ✓    │
│  Output Directory: .next              ✓    │
│  Install Command: pnpm install        ✓    │
│                                             │
│  [Cancel]              [Deploy]             │
└─────────────────────────────────────────────┘
```

---

## Verification Steps

After deploying, verify:

✅ **Build succeeds**
   - Check deployment logs
   - No "package.json not found" errors

✅ **Dashboard loads**
   - Visit your Vercel URL
   - Home page displays

✅ **All pages work**
   - Dashboard, Students, Books, Transactions, Alerts

✅ **Firebase connected**
   - Stats display correctly
   - Real-time updates work

---

## Quick Commands Reference

### Deploy from web-dashboard folder
```bash
cd web-dashboard
vercel login
vercel                    # Preview deployment
vercel --prod            # Production deployment
```

### Check deployment status
```bash
vercel ls                # List deployments
vercel logs              # View logs
```

### Add environment variables
```bash
vercel env add NEXT_PUBLIC_FIREBASE_API_KEY
# Enter value when prompted
# Select: Production, Preview, Development (choose all)
```

### Redeploy
```bash
vercel --prod --force    # Force new production deployment
```

---

## Troubleshooting

### Still getting "No package.json found"?

**Check 1**: Root directory is set correctly
- Go to Project Settings in Vercel
- Build & Development Settings
- Root Directory should show: `web-dashboard`

**Check 2**: You're in the right folder
```bash
pwd
# Should show: .../web-dashboard
ls
# Should show: package.json, app/, components/, etc.
```

**Check 3**: Redeploy after changing settings
- Make a small change
- Push to GitHub
- Or run `vercel --prod --force`

---

## Project Structure

Your project structure:
```
Smart Library Management System/
├── src/                          # ESP32 code
├── platformio.ini
├── web-dashboard/                # ← Vercel needs to deploy THIS folder
│   ├── package.json             # ← Vercel looks for this
│   ├── app/
│   ├── components/
│   ├── lib/
│   └── ...
├── README.md
└── VERCEL_DEPLOY.md
```

Vercel **must** know to look in `web-dashboard` folder!

---

## Success!

When configured correctly, you'll see:

```
✓ Deployment ready [2m]
✓ Build completed
✓ Assigned custom domains

Production: https://smart-library-dashboard.vercel.app
```

🎉 **Your dashboard is now live!**

---

## Need Help?

- [Vercel Root Directory Docs](https://vercel.com/docs/concepts/projects/project-configuration#root-directory)
- [Next.js Deployment Guide](https://nextjs.org/docs/deployment)
- Check [DEPLOY_TO_VERCEL.md](./DEPLOY_TO_VERCEL.md) for detailed guide

---

**Remember**: Always deploy from the `web-dashboard` folder or set Root Directory to `web-dashboard` in Vercel settings!
