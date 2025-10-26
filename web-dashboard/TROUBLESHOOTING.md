# 🔧 Vercel Deployment Troubleshooting

## Common Issues & Solutions

### Issue 1: "No package.json found" ❌

**Symptoms:**
```
Error: Could not find package.json
Build failed
```

**Root Cause:**
Vercel is looking in the wrong directory (project root instead of `web-dashboard/`)

**Solution:**

**Method A: Via Vercel Dashboard** (Easiest)
1. Go to your project on Vercel
2. Settings → General
3. Find "Root Directory"
4. Click "Edit"
5. Enter: `web-dashboard`
6. Click "Save"
7. Go to Deployments tab
8. Click "Redeploy" on the latest deployment

**Method B: Delete and Reimport**
1. Delete the project from Vercel
2. Import again from GitHub
3. **DON'T SKIP**: Set Root Directory to `web-dashboard` during import
4. Deploy

---

### Issue 2: Build succeeds but shows 404 ❌

**Symptoms:**
- Build completes successfully
- But visiting the URL shows "404 - Not Found"

**Root Cause:**
Output directory mismatch or routing issue

**Solution:**
1. Check Project Settings → Build & Development Settings
2. Output Directory should be: `.next`
3. If different, change it to `.next`
4. Redeploy

---

### Issue 3: Firebase not connecting ❌

**Symptoms:**
- Dashboard loads but shows "Loading..." forever
- No data displays
- Console shows Firebase errors

**Root Cause:**
Missing or incorrect environment variables

**Solution:**
1. Go to Project Settings → Environment Variables
2. Verify ALL 7 variables exist:
   ```
   NEXT_PUBLIC_FIREBASE_API_KEY
   NEXT_PUBLIC_FIREBASE_AUTH_DOMAIN
   NEXT_PUBLIC_FIREBASE_DATABASE_URL
   NEXT_PUBLIC_FIREBASE_PROJECT_ID
   NEXT_PUBLIC_FIREBASE_STORAGE_BUCKET
   NEXT_PUBLIC_FIREBASE_MESSAGING_SENDER_ID
   NEXT_PUBLIC_FIREBASE_APP_ID
   ```
3. Check values match your `.env.local` file
4. Make sure they're set for **Production** environment
5. Redeploy after adding/fixing variables

---

### Issue 4: pnpm/npm install fails ❌

**Symptoms:**
```
ERR_PNPM_META_FETCH_FAIL
or
npm ERR! network timeout
```

**Root Cause:**
Network issues or package manager conflicts

**Solution:**
1. Vercel automatically uses npm by default (good!)
2. Make sure `pnpm-lock.yaml` is NOT in your repo
3. Only `package.json` should be there
4. If you have pnpm files, delete them:
   ```bash
   cd web-dashboard
   rm -f pnpm-lock.yaml pnpm-workspace.yaml
   git add .
   git commit -m "Remove pnpm lock files"
   git push
   ```
5. Vercel will auto-redeploy with npm

---

### Issue 5: Build works locally but fails on Vercel ❌

**Symptoms:**
- `npm run build` works on your computer
- Fails on Vercel with TypeScript or build errors

**Root Cause:**
- Different Node.js versions
- Missing dependencies
- TypeScript strictness

**Solution:**
1. Check Node.js version:
   - Vercel uses Node 18 by default
2. Add `.nvmrc` file in `web-dashboard/`:
   ```
   18
   ```
3. Or specify in `package.json`:
   ```json
   "engines": {
     "node": ">=18.0.0"
   }
   ```
4. Push changes and redeploy

---

### Issue 6: Environment variables not working ❌

**Symptoms:**
- Added variables but still getting undefined
- Firebase still not connecting

**Root Cause:**
- Variables not applied to correct environment
- Not redeployed after adding variables
- Missing `NEXT_PUBLIC_` prefix

**Solution:**
1. **All** variables MUST start with `NEXT_PUBLIC_` for client-side access
2. After adding variables, you MUST redeploy:
   - Go to Deployments
   - Click "..." menu on latest deployment
   - Click "Redeploy"
3. Check variables are set for **Production** (not just Preview/Development)

---

### Issue 7: Deployment stuck or taking too long ❌

**Symptoms:**
- Deployment running for 10+ minutes
- Seems stuck on "Building..."

**Solution:**
1. Cancel the deployment
2. Check your code for infinite loops in build scripts
3. Redeploy
4. If still stuck, contact Vercel support

---

## 🎯 Pre-Deployment Checklist

Before deploying, verify:

- [ ] `package.json` exists in `web-dashboard/` folder
- [ ] Root Directory is set to `web-dashboard` in Vercel
- [ ] All 7 environment variables are added
- [ ] Environment variables have `NEXT_PUBLIC_` prefix
- [ ] No `pnpm-lock.yaml` in repository
- [ ] Code is pushed to GitHub
- [ ] Firebase Database URL is correct

---

## ✅ How to Verify Successful Deployment

1. **Build Logs** should show:
   ```
   ✓ Creating an optimized production build
   ✓ Compiled successfully
   ✓ Build completed
   ```

2. **Visit URL** - Dashboard should load

3. **Check Console** (F12 → Console):
   - No red errors
   - Firebase connects successfully

4. **Test Features**:
   - Navigation works
   - Data displays from Firebase
   - Real-time updates work with ESP32

---

## 🆘 Ultimate Fix: Deploy from CLI

If dashboard deployment keeps failing, use CLI method:

```bash
# Make sure you're in the web-dashboard folder
cd web-dashboard

# Install Vercel CLI globally
npm install -g vercel

# Login
vercel login

# Deploy
vercel --prod
```

This method bypasses the root directory issue entirely!

**Add environment variables via CLI:**
```bash
vercel env add NEXT_PUBLIC_FIREBASE_API_KEY
# Enter value when prompted
# Select: Production, Preview, Development (all)

# Repeat for all 7 variables
```

Then redeploy:
```bash
vercel --prod
```

---

## 📊 Deployment Logs

To view detailed logs:

1. Go to Vercel Dashboard
2. Click your project
3. Click on the deployment
4. Click "View Build Logs"
5. Scroll through to find errors

Common log patterns:

**Good (Working):**
```
✓ Linting and checking validity
✓ Creating an optimized production build
✓ Compiled successfully
✓ Deployment ready
```

**Bad (Failing):**
```
✗ npm ERR! ...
✗ Error: ...
✗ Build failed
```

---

## 💡 Pro Tips

1. **Always set Root Directory first** before clicking Deploy
2. **Add env variables during initial setup**, not after
3. **Use npm** not pnpm for Vercel deployments
4. **Test locally** before deploying: `npm run build`
5. **Check Firebase Console** - make sure database has data
6. **Redeploy after ANY settings change**

---

## 🎓 Learn More

- [Vercel Next.js Deployment](https://vercel.com/docs/frameworks/nextjs)
- [Vercel Environment Variables](https://vercel.com/docs/concepts/projects/environment-variables)
- [Next.js Documentation](https://nextjs.org/docs/deployment)

---

**Remember:** 90% of deployment issues are due to **Root Directory not being set to `web-dashboard`**!

Always check this first! 🎯
