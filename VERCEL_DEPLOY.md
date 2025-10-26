# 🚀 Deploy Your Smart Library Dashboard to Vercel

## Quick Deploy (5 Minutes)

Your Next.js dashboard is ready to deploy to Vercel - **completely FREE**!

### Option 1: One-Click Deploy (Easiest)

[![Deploy with Vercel](https://vercel.com/button)](https://vercel.com/new/clone)

Click the button above and follow the prompts!

---

## Option 2: Manual Deploy (Recommended)

### Step 1: Install Vercel CLI

```bash
pnpm install -g vercel
```

### Step 2: Navigate to Dashboard Folder

**IMPORTANT**: You must deploy from the `web-dashboard` folder:

```bash
cd web-dashboard
```

### Step 3: Login to Vercel

```bash
vercel login
```

Follow the browser login prompt.

### Step 4: Deploy

```bash
vercel
```

Answer the prompts:
- **Set up and deploy?** → `Y`
- **Which scope?** → Select your account
- **Link to existing project?** → `N`
- **Project name?** → `smart-library-dashboard`
- **In which directory is your code located?** → `./`
- **Want to override the settings?** → `N`

### Step 5: Add Environment Variables

After deployment, add your Firebase credentials:

```bash
vercel env add NEXT_PUBLIC_FIREBASE_API_KEY production
# Enter: AIzaSyCI0gN-FwbTzfS43_LURWjHAr_JfwDaglM

vercel env add NEXT_PUBLIC_FIREBASE_DATABASE_URL production
# Enter: https://smart-library-using-esp32-default-rtdb.asia-southeast1.firebasedatabase.app

vercel env add NEXT_PUBLIC_FIREBASE_PROJECT_ID production
# Enter: smart-library-using-esp32

vercel env add NEXT_PUBLIC_FIREBASE_AUTH_DOMAIN production
# Enter: smart-library-using-esp32.firebaseapp.com

vercel env add NEXT_PUBLIC_FIREBASE_STORAGE_BUCKET production
# Enter: smart-library-using-esp32.appspot.com

vercel env add NEXT_PUBLIC_FIREBASE_MESSAGING_SENDER_ID production
# Enter: your-sender-id

vercel env add NEXT_PUBLIC_FIREBASE_APP_ID production
# Enter: your-app-id
```

### Step 6: Deploy to Production

```bash
vercel --prod
```

🎉 **Done!** Your dashboard is live at: `https://your-project.vercel.app`

---

## Option 3: Deploy via GitHub (Best for Teams)

### Step 1: Push to GitHub

```bash
# From project root
cd "/Users/koyyalasaipreetham/Documents/PlatformIO/Projects/Smart Library Management and Monitoring System using ESP32"

# Initialize git if needed
git init

# Add files
git add .
git commit -m "Add web dashboard for Smart Library System"

# Create a repository on GitHub, then:
git remote add origin https://github.com/YOUR_USERNAME/YOUR_REPO.git
git push -u origin main
```

### Step 2: Import to Vercel

1. Go to [vercel.com](https://vercel.com)
2. Click **"Add New Project"**
3. Import your GitHub repository
4. **⚠️ CRITICAL**: Click **"Edit"** next to **Root Directory**
5. Set Root Directory to: `web-dashboard`
6. Click **"Continue"**
7. Add environment variables (see list above)
8. Click **"Deploy"**

**If you skip step 4-5, deployment will fail!**

---

## Your Deployment Checklist

- [ ] Vercel account created
- [ ] Code pushed to GitHub (optional)
- [ ] Vercel CLI installed (`pnpm install -g vercel`)
- [ ] Logged in to Vercel (`vercel login`)
- [ ] Deployed with `vercel`
- [ ] Environment variables added
- [ ] Production deployment (`vercel --prod`)
- [ ] Dashboard accessible via URL

---

## Environment Variables Required

Make sure to add these in Vercel:

```
NEXT_PUBLIC_FIREBASE_API_KEY
NEXT_PUBLIC_FIREBASE_AUTH_DOMAIN
NEXT_PUBLIC_FIREBASE_DATABASE_URL
NEXT_PUBLIC_FIREBASE_PROJECT_ID
NEXT_PUBLIC_FIREBASE_STORAGE_BUCKET
NEXT_PUBLIC_FIREBASE_MESSAGING_SENDER_ID
NEXT_PUBLIC_FIREBASE_APP_ID
```

Get values from your Firebase Console or from `web-dashboard/.env.local`

---

## After Deployment

### Test Your Live Dashboard

1. Open your Vercel URL: `https://your-project.vercel.app`
2. Check if data loads from Firebase
3. Test with your ESP32 device
4. Verify real-time updates work

### Share Your Dashboard

Your dashboard is now accessible from:
- 📱 Any mobile device
- 💻 Any computer
- 🌍 Anywhere in the world!

Share the URL with librarians, students, or anyone who needs access.

---

## Automatic Updates

Every time you push to GitHub:
1. Vercel detects the change
2. Automatically builds the app
3. Deploys the update
4. **Zero downtime!** 🚀

---

## Custom Domain (Optional)

Want a custom URL like `library.yourdomain.com`?

1. Go to Vercel Dashboard
2. Select your project
3. Go to **Settings** → **Domains**
4. Add your domain
5. Update DNS records (Vercel provides instructions)

---

## Cost

**Completely FREE** for this project!

Vercel Free Plan includes:
- Unlimited deployments
- 100GB bandwidth/month
- Automatic HTTPS
- Global CDN
- More than enough for a library system!

---

## Troubleshooting

### Build fails?

Check build logs in Vercel dashboard. Common issues:
- Missing environment variables
- TypeScript errors
- Wrong root directory (should be `web-dashboard`)

### Can't access Firebase?

1. Check environment variables are set correctly
2. Verify Firebase security rules allow read access
3. Check Firebase Console → Realtime Database → Rules

### Updates not deploying?

```bash
# Force a new deployment
vercel --force --prod
```

---

## Detailed Guide

For step-by-step screenshots and detailed instructions, see:
- [web-dashboard/DEPLOY_TO_VERCEL.md](web-dashboard/DEPLOY_TO_VERCEL.md)

---

## Support

- **Vercel Docs**: https://vercel.com/docs
- **Next.js Docs**: https://nextjs.org/docs
- **Firebase Docs**: https://firebase.google.com/docs

---

**Your Smart Library Dashboard is production-ready!** 🎉

Deploy it and access your library management system from anywhere in the world!
