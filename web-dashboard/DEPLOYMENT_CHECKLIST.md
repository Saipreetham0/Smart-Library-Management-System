# Vercel Deployment Checklist

Use this checklist to ensure a smooth deployment to Vercel.

## Pre-Deployment

- [ ] **Node.js installed** (v18 or higher)
  ```bash
  node --version
  ```

- [ ] **pnpm installed**
  ```bash
  pnpm --version
  ```

- [ ] **Dependencies installed**
  ```bash
  pnpm install
  ```

- [ ] **Local build successful**
  ```bash
  pnpm build
  ```

- [ ] **Local app runs without errors**
  ```bash
  pnpm dev
  # Visit http://localhost:3000
  ```

- [ ] **Firebase credentials ready**
  - Check `.env.local` file has all values
  - Copy these for Vercel later

## Deployment Options

Choose one method:

### Option A: GitHub + Vercel Dashboard ⭐ Recommended

- [ ] Create GitHub repository
- [ ] Push code to GitHub
  ```bash
  git init
  git add .
  git commit -m "Add Smart Library Dashboard"
  git remote add origin YOUR_GITHUB_URL
  git push -u origin main
  ```
- [ ] Sign up/Login to [vercel.com](https://vercel.com)
- [ ] Import GitHub repository
- [ ] Set Root Directory to `web-dashboard`
- [ ] Add environment variables
- [ ] Deploy

### Option B: Vercel CLI

- [ ] Install Vercel CLI
  ```bash
  pnpm install -g vercel
  ```
- [ ] Login to Vercel
  ```bash
  vercel login
  ```
- [ ] Deploy
  ```bash
  cd web-dashboard
  vercel
  ```
- [ ] Add environment variables via CLI
- [ ] Deploy to production
  ```bash
  vercel --prod
  ```

## Environment Variables to Add

Copy from your `.env.local` file:

- [ ] `NEXT_PUBLIC_FIREBASE_API_KEY`
- [ ] `NEXT_PUBLIC_FIREBASE_AUTH_DOMAIN`
- [ ] `NEXT_PUBLIC_FIREBASE_DATABASE_URL`
- [ ] `NEXT_PUBLIC_FIREBASE_PROJECT_ID`
- [ ] `NEXT_PUBLIC_FIREBASE_STORAGE_BUCKET`
- [ ] `NEXT_PUBLIC_FIREBASE_MESSAGING_SENDER_ID`
- [ ] `NEXT_PUBLIC_FIREBASE_APP_ID`

## Post-Deployment Testing

- [ ] **Deployment successful**
  - No build errors
  - Received deployment URL

- [ ] **Dashboard loads**
  - Visit your Vercel URL
  - No console errors (F12 → Console)

- [ ] **Firebase connection works**
  - Stats display correctly
  - Data from ESP32 visible

- [ ] **Navigation works**
  - [ ] Dashboard page
  - [ ] Students page
  - [ ] Books page
  - [ ] Transactions page
  - [ ] Alerts page

- [ ] **Real-time updates work**
  - Scan RFID card on ESP32
  - Dashboard updates automatically

- [ ] **Mobile responsive**
  - Test on phone/tablet
  - UI looks good

## Firebase Configuration

- [ ] **Security rules configured**
  ```json
  {
    "rules": {
      ".read": true,
      ".write": true
    }
  }
  ```

- [ ] **Vercel domain added to Firebase**
  - Firebase Console → Project Settings
  - Authorized domains → Add Vercel domain

## Performance Checks

- [ ] **Lighthouse score** (optional)
  - Chrome DevTools → Lighthouse
  - Run audit
  - Should be 90+ for Performance

- [ ] **Core Web Vitals** (optional)
  - Check Vercel Analytics
  - Monitor performance

## Documentation

- [ ] **Save deployment URL**
  - Bookmark for easy access
  - Share with team

- [ ] **Update README** (optional)
  - Add live demo link

- [ ] **Document any custom settings**
  - Note any configuration changes

## Ongoing Maintenance

- [ ] **Enable automatic deployments**
  - Push to GitHub → Auto deploy

- [ ] **Monitor deployments**
  - Check Vercel dashboard regularly

- [ ] **Update dependencies** (monthly)
  ```bash
  pnpm update
  ```

## Troubleshooting

If deployment fails, check:

- [ ] Build logs in Vercel dashboard
- [ ] All environment variables are set
- [ ] Root directory is `web-dashboard`
- [ ] No TypeScript errors locally
- [ ] Firebase credentials are correct

## Emergency Rollback

If something breaks:

- [ ] Go to Vercel dashboard
- [ ] Deployments tab
- [ ] Find last working deployment
- [ ] Click "Promote to Production"

## Success Criteria

✅ Your deployment is successful when:

1. Dashboard loads without errors
2. Firebase data displays correctly
3. Real-time updates work with ESP32
4. All pages are accessible
5. Mobile version works well
6. HTTPS is enabled (automatic)
7. URL is shareable

## Quick Commands Reference

```bash
# Install dependencies
pnpm install

# Test locally
pnpm dev

# Build for production
pnpm build

# Deploy to Vercel preview
vercel

# Deploy to Vercel production
vercel --prod

# Add environment variable
vercel env add VARIABLE_NAME

# View deployments
vercel ls

# View logs
vercel logs
```

## Support Resources

- **Vercel Docs**: https://vercel.com/docs
- **Next.js Docs**: https://nextjs.org/docs
- **Firebase Docs**: https://firebase.google.com/docs
- **Deployment Guide**: [DEPLOY_TO_VERCEL.md](./DEPLOY_TO_VERCEL.md)

---

## Completion

When all checkboxes are checked:

🎉 **Congratulations!** Your Smart Library Dashboard is live on Vercel!

Share your deployment URL:
```
https://your-project.vercel.app
```

Access from anywhere:
- Desktop 💻
- Mobile 📱
- Tablet 📲
- Anywhere with internet 🌍

---

**Last Updated**: Check this list before every deployment
**Time to Deploy**: ~10 minutes for first deployment
**Cost**: $0 (Free Forever)
