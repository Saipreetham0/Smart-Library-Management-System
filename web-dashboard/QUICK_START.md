# Quick Start - 3 Steps

Get your dashboard running in minutes!

## 1. Install pnpm (if you don't have it)

```bash
npm install -g pnpm
```

## 2. Install dependencies

```bash
cd web-dashboard
pnpm install
```

## 3. Run the dashboard

```bash
pnpm dev
```

That's it! Open **http://localhost:3000** in your browser.

---

## Why pnpm?

- **Faster** - Up to 2x faster than npm
- **Efficient** - Saves disk space by using hard links
- **Reliable** - Strict dependency resolution

## First Time Setup?

If this is your first time running the dashboard:

1. Make sure you have the `.env.local` file with Firebase credentials
2. If not, copy `.env.example` to `.env.local`:
   ```bash
   cp .env.example .env.local
   ```

## ESP32 Connected?

Make sure your ESP32 is:
- ✅ Powered on
- ✅ Connected to WiFi
- ✅ Writing to Firebase

You should see real-time updates in the dashboard as you scan cards!

## Need More Help?

- Detailed setup: See [SETUP_GUIDE.md](./SETUP_GUIDE.md)
- Full documentation: See [README.md](./README.md)

---

**Happy library managing!** 📚
