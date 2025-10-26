# Deploy to Vercel - Step by Step Guide

Deploy your Smart Library Dashboard to Vercel in minutes - completely **FREE**!

## Why Vercel?

- ✅ **Free hosting** for Next.js apps
- ✅ **Automatic HTTPS** with SSL certificate
- ✅ **Global CDN** for fast loading worldwide
- ✅ **Auto-deploy** on every git push
- ✅ **Zero configuration** - works out of the box
- ✅ Built by the creators of Next.js

---

## Method 1: Deploy via Vercel Dashboard (Easiest)

### Step 1: Create a GitHub Repository

1. Go to [GitHub](https://github.com) and create a new repository
2. Name it: `smart-library-dashboard` (or any name you prefer)
3. Make it **Public** or **Private** (your choice)

### Step 2: Push Your Code to GitHub

Open terminal in your project root directory:

```bash
# Navigate to project root (not web-dashboard folder)
cd "/Users/koyyalasaipreetham/Documents/PlatformIO/Projects/Smart Library Management and Monitoring System using ESP32"

# Initialize git (if not already done)
git init

# Add web-dashboard to git
git add web-dashboard/
git add WEB_DASHBOARD_INFO.md

# Commit
git commit -m "Add Next.js web dashboard"

# Add your GitHub repository as remote
git remote add origin https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git

# Push to GitHub
git push -u origin main
```

### Step 3: Deploy on Vercel

1. Go to [Vercel](https://vercel.com)
2. Click **"Sign Up"** or **"Log In"** (use GitHub account - it's easier)
3. Click **"Add New Project"**
4. Click **"Import Git Repository"**
5. Select your repository: `smart-library-dashboard`
6. Vercel will auto-detect Next.js ✅

### Step 4: Configure Root Directory

**IMPORTANT**: Since your Next.js app is in the `web-dashboard` folder:

1. In the import screen, click **"Edit"** next to Root Directory
2. Set Root Directory to: `web-dashboard`
3. Click **"Continue"**

### Step 5: Add Environment Variables

1. Click **"Environment Variables"** section
2. Add each variable from your `.env.local` file:

```
NEXT_PUBLIC_FIREBASE_API_KEY = AIzaSyCI0gN-FwbTzfS43_LURWjHAr_JfwDaglM
NEXT_PUBLIC_FIREBASE_AUTH_DOMAIN = smart-library-using-esp32.firebaseapp.com
NEXT_PUBLIC_FIREBASE_DATABASE_URL = https://smart-library-using-esp32-default-rtdb.asia-southeast1.firebasedatabase.app
NEXT_PUBLIC_FIREBASE_PROJECT_ID = smart-library-using-esp32
NEXT_PUBLIC_FIREBASE_STORAGE_BUCKET = smart-library-using-esp32.appspot.com
NEXT_PUBLIC_FIREBASE_MESSAGING_SENDER_ID = 123456789
NEXT_PUBLIC_FIREBASE_APP_ID = 1:123456789:web:abcdef
```

> **Note**: Use your actual Firebase credentials

### Step 6: Deploy!

1. Click **"Deploy"**
2. Wait 1-2 minutes while Vercel builds your app
3. 🎉 **Done!** You'll get a URL like: `https://your-app.vercel.app`

---

## Method 2: Deploy via Vercel CLI (For Advanced Users)

### Step 1: Install Vercel CLI

```bash
npm install -g vercel
```

### Step 2: Login to Vercel

```bash
vercel login
```

### Step 3: Deploy

```bash
cd web-dashboard
vercel
```

Follow the prompts:
- **Set up and deploy?** → Yes
- **Which scope?** → Your account
- **Link to existing project?** → No
- **Project name?** → smart-library-dashboard
- **Directory?** → `./` (already in web-dashboard)
- **Want to override settings?** → No

### Step 4: Add Environment Variables

```bash
# Add each environment variable
vercel env add NEXT_PUBLIC_FIREBASE_API_KEY
vercel env add NEXT_PUBLIC_FIREBASE_AUTH_DOMAIN
vercel env add NEXT_PUBLIC_FIREBASE_DATABASE_URL
vercel env add NEXT_PUBLIC_FIREBASE_PROJECT_ID
vercel env add NEXT_PUBLIC_FIREBASE_STORAGE_BUCKET
vercel env add NEXT_PUBLIC_FIREBASE_MESSAGING_SENDER_ID
vercel env add NEXT_PUBLIC_FIREBASE_APP_ID
```

### Step 5: Redeploy with Environment Variables

```bash
vercel --prod
```

---

## After Deployment

### Your Live URL

You'll get a URL like:
```
https://smart-library-dashboard.vercel.app
```

### Custom Domain (Optional)

1. Go to Vercel Dashboard → Your Project
2. Click **"Settings"** → **"Domains"**
3. Add your custom domain (if you have one)

### Automatic Updates

Every time you push to GitHub:
1. Vercel automatically detects changes
2. Builds your app
3. Deploys the update
4. Zero downtime! 🚀

---

## Troubleshooting

### Build Failed?

Check the build logs in Vercel dashboard:
1. Click on your deployment
2. View **"Build Logs"**
3. Look for errors

Common issues:
- Missing environment variables
- Wrong root directory
- TypeScript errors

### Root Directory Not Set?

If you see "No package.json found":
1. Go to Project Settings
2. Set **Root Directory** to `web-dashboard`
3. Redeploy

### Environment Variables Not Working?

1. Go to Project Settings → Environment Variables
2. Make sure all variables start with `NEXT_PUBLIC_`
3. Redeploy after adding variables

---

## Firebase Configuration for Production

### Update Firebase Security Rules

For production, update your Firebase Realtime Database rules:

```json
{
  "rules": {
    ".read": true,
    ".write": "auth != null"
  }
}
```

Or for public read-only:
```json
{
  "rules": {
    ".read": true,
    ".write": false
  }
}
```

### Enable CORS (if needed)

If you see CORS errors, add your Vercel domain to Firebase:
1. Firebase Console → Project Settings
2. Scroll to "Authorized domains"
3. Add your Vercel domain: `your-app.vercel.app`

---

## Cost

**100% FREE** for:
- Unlimited deployments
- Automatic HTTPS
- Global CDN
- 100GB bandwidth/month
- Perfect for this project!

---

## Monitoring

### View Analytics

1. Go to Vercel Dashboard
2. Click your project
3. View **"Analytics"** tab
4. See visitor stats, performance metrics

### View Logs

1. Click on a deployment
2. View **"Functions"** tab
3. See real-time logs

---

## Update Your Dashboard

To deploy updates:

```bash
# Make changes to your code
# Commit and push to GitHub
git add .
git commit -m "Update dashboard"
git push

# Vercel auto-deploys! 🎉
```

---

## Quick Reference

| Task | Command |
|------|---------|
| Deploy to Vercel | Push to GitHub (auto-deploys) |
| Deploy manually | `vercel --prod` |
| View deployments | Visit Vercel dashboard |
| Rollback | Vercel dashboard → Deployments → Promote |
| View logs | Vercel dashboard → Functions |

---

## Next Steps After Deployment

1. ✅ Share your live URL with users
2. ✅ Bookmark the dashboard
3. ✅ Test with your ESP32 device
4. ✅ Monitor via Vercel analytics
5. ✅ Add custom domain (optional)

---

**Your Smart Library Dashboard is now live on the internet!** 🌐

Access it from anywhere:
- Your phone 📱
- Your computer 💻
- Any device with internet 🌍

Share the URL with librarians, students, or anyone who needs access!
