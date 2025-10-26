# Quick Setup Guide

Follow these steps to get your Smart Library Dashboard up and running.

## Step 1: Install Node.js and pnpm

If you don't have Node.js installed:

1. Visit [nodejs.org](https://nodejs.org/)
2. Download and install Node.js 18 or higher
3. Install pnpm globally:
   ```bash
   npm install -g pnpm
   ```
4. Verify installation:
   ```bash
   node --version
   pnpm --version
   ```

## Step 2: Install Dependencies

Open terminal in the `web-dashboard` folder and run:

```bash
pnpm install
```

This will install all required packages. It may take a few minutes.

## Step 3: Configure Firebase

### Option A: Use Existing Configuration

The project already has Firebase credentials configured for the ESP32 project. Just create `.env.local`:

```bash
cp .env.example .env.local
```

### Option B: Use Your Own Firebase Project

1. Go to [Firebase Console](https://console.firebase.google.com/)
2. Create a new project or select existing
3. Enable Realtime Database
4. Get your config from Project Settings
5. Update `.env.local` with your credentials

## Step 4: Run the Dashboard

Start the development server:

```bash
pnpm dev
```

Open your browser to: **http://localhost:3000**

## Step 5: Test with ESP32

1. Make sure your ESP32 is powered on and connected to WiFi
2. ESP32 should be writing to Firebase
3. You should see real-time updates in the dashboard

## Common Issues

### "Cannot find module" errors
```bash
rm -rf node_modules pnpm-lock.yaml
pnpm install
```

### Port 3000 already in use
```bash
# Kill the process using port 3000 or use a different port
pnpm dev -- -p 3001
```

### No data showing
1. Check ESP32 serial monitor - is it connected to Firebase?
2. Open Firebase Console - is data being written?
3. Check browser console (F12) for errors
4. Verify `.env.local` has correct Firebase URL

## Firebase Database Rules

For development, use these rules (Firebase Console → Realtime Database → Rules):

```json
{
  "rules": {
    ".read": true,
    ".write": true
  }
}
```

⚠️ **Warning**: These rules are open. For production, implement proper security rules.

## Quick Test

1. **Dashboard**: Should show stats (may be 0 if no data yet)
2. **Students**: Should show student list from ESP32
3. **Books**: Should show book inventory
4. **Transactions**: Should show activity as you scan cards

## Adding Test Data

If you want to add data manually:

1. Go to **Students** page
2. Click "Add Student"
3. Fill in:
   - Student ID: S004
   - Name: Test Student
   - RFID Card: 12345678
4. Click "Add Student"

Do the same for **Books** page.

## Production Deployment

When ready to deploy:

```bash
pnpm build
```

Then deploy to:
- **Vercel**: Recommended (free, easy setup)
- **Netlify**: Good alternative
- **Self-hosted**: Use `pnpm start` after build

## Need Help?

Check the full [README.md](./README.md) for detailed documentation.

## Verification Checklist

- [ ] Node.js installed (version 18+)
- [ ] pnpm installed (`npm install -g pnpm`)
- [ ] Dependencies installed (`pnpm install`)
- [ ] `.env.local` created with Firebase config
- [ ] Development server running (`pnpm dev`)
- [ ] Dashboard opens in browser
- [ ] ESP32 connected and writing to Firebase
- [ ] Real-time updates working

If all checked, you're good to go!
