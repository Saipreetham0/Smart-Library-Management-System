# Web Dashboard - Smart Library Management System

A modern Next.js web application has been created for your ESP32 Smart Library Management System!

## Location

```
web-dashboard/
```

## What's Included

### 5 Main Pages:
1. **Dashboard** - Real-time stats and activity feed
2. **Students** - Manage students and RFID cards
3. **Books** - Manage book inventory and NFC tags
4. **Transactions** - View complete transaction history
5. **Alerts** - Monitor noise levels and alerts

### Features:
- ✅ Real-time Firebase synchronization
- ✅ Add, edit, delete students and books
- ✅ Search and filter functionality
- ✅ Responsive design (mobile, tablet, desktop)
- ✅ Modern UI with Tailwind CSS
- ✅ TypeScript for type safety

## Quick Start (3 Steps)

```bash
# 1. Install pnpm (if needed)
npm install -g pnpm

# 2. Navigate and install
cd web-dashboard
pnpm install

# 3. Run the dashboard
pnpm dev
```

Open **http://localhost:3000**

## Technology Stack

- **Framework**: Next.js 14 (React)
- **Language**: TypeScript
- **Styling**: Tailwind CSS
- **Database**: Firebase Realtime Database
- **Package Manager**: pnpm

## Firebase Integration

The dashboard uses the **same Firebase credentials** as your ESP32:
- API Key: Already configured
- Database URL: `https://smart-library-using-esp32-default-rtdb.asia-southeast1.firebasedatabase.app`

No additional Firebase setup needed - it uses your existing project!

## How It Works

```
ESP32 Device
    ↓ (WiFi)
Firebase Realtime Database
    ↓ (Real-time sync)
Web Dashboard
```

When you scan an RFID/NFC card on the ESP32:
1. ESP32 writes data to Firebase
2. Dashboard receives update in real-time
3. UI updates automatically - no refresh needed!

## File Structure

```
web-dashboard/
├── app/                    # Pages
│   ├── page.tsx           # Dashboard
│   ├── students/          # Students page
│   ├── books/             # Books page
│   ├── transactions/      # Transactions page
│   └── alerts/            # Alerts page
├── components/            # Reusable components
├── lib/                   # Firebase & utilities
├── QUICK_START.md        # 3-step guide
├── SETUP_GUIDE.md        # Detailed setup
└── README.md             # Full documentation
```

## Documentation

- **Quick Start**: [web-dashboard/QUICK_START.md](web-dashboard/QUICK_START.md)
- **Setup Guide**: [web-dashboard/SETUP_GUIDE.md](web-dashboard/SETUP_GUIDE.md)
- **Full README**: [web-dashboard/README.md](web-dashboard/README.md)

## Deployment

When ready to deploy to production:

```bash
pnpm build
```

Deploy to:
- **Vercel** (recommended - free, optimized for Next.js)
- **Netlify**
- **AWS Amplify**
- Any Node.js hosting

## Troubleshooting

### No data showing?
1. Check ESP32 is connected to WiFi
2. Verify Firebase Console shows data
3. Ensure `.env.local` has correct credentials

### Port already in use?
```bash
pnpm dev -- -p 3001
```

### Module errors?
```bash
rm -rf node_modules pnpm-lock.yaml
pnpm install
```

## Next Steps

### Local Development
1. ✅ Run `pnpm install` in web-dashboard folder
2. ✅ Start with `pnpm dev`
3. ✅ Test with your ESP32 device
4. ✅ Scan RFID/NFC cards and watch real-time updates
5. ✅ Add/edit students and books through the web UI

### Deploy to Production (FREE)
1. ✅ See [VERCEL_DEPLOY.md](VERCEL_DEPLOY.md) for quick deploy guide
2. ✅ Or use: `cd web-dashboard && vercel`
3. ✅ Your dashboard will be live in 2 minutes!
4. ✅ Access from anywhere: `https://your-project.vercel.app`

## Deployment

The dashboard is **ready to deploy to Vercel** (100% free):

- **Quick Deploy**: See [VERCEL_DEPLOY.md](VERCEL_DEPLOY.md)
- **Detailed Guide**: See [web-dashboard/DEPLOY_TO_VERCEL.md](web-dashboard/DEPLOY_TO_VERCEL.md)
- **Checklist**: See [web-dashboard/DEPLOYMENT_CHECKLIST.md](web-dashboard/DEPLOYMENT_CHECKLIST.md)

**Deploy in 3 commands**:
```bash
cd web-dashboard
pnpm install -g vercel
vercel --prod
```

## Support

For detailed information, check the documentation files in the `web-dashboard` folder.

---

**Your Smart Library Management System now has a professional web interface!** 🎉

**Ready for production deployment to Vercel!** 🚀
