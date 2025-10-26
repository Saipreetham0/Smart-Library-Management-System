#!/bin/bash

# Smart Library Dashboard - Vercel Deployment Script

echo "================================================"
echo "  Smart Library Dashboard - Vercel Deployment"
echo "================================================"
echo ""

# Check if vercel CLI is installed
if ! command -v vercel &> /dev/null; then
    echo "⚠️  Vercel CLI is not installed"
    echo ""
    echo "Installing Vercel CLI..."
    pnpm install -g vercel

    if [ $? -ne 0 ]; then
        echo "❌ Failed to install Vercel CLI"
        echo "   Please install manually: pnpm install -g vercel"
        exit 1
    fi
    echo "✅ Vercel CLI installed"
fi

echo "✅ Vercel CLI is ready"
echo ""

# Check if user is logged in
echo "Checking Vercel login status..."
vercel whoami &> /dev/null

if [ $? -ne 0 ]; then
    echo "⚠️  Not logged in to Vercel"
    echo ""
    echo "Please log in to Vercel..."
    vercel login

    if [ $? -ne 0 ]; then
        echo "❌ Login failed"
        exit 1
    fi
fi

echo "✅ Logged in to Vercel"
echo ""

# Ask deployment type
echo "Select deployment type:"
echo "  1) Preview (test deployment)"
echo "  2) Production (live deployment)"
echo ""
read -p "Enter choice (1 or 2): " choice

echo ""

case $choice in
    1)
        echo "🚀 Deploying to Preview..."
        vercel
        ;;
    2)
        echo "🚀 Deploying to Production..."
        vercel --prod
        ;;
    *)
        echo "❌ Invalid choice"
        exit 1
        ;;
esac

if [ $? -eq 0 ]; then
    echo ""
    echo "================================================"
    echo "  ✅ Deployment Successful!"
    echo "================================================"
    echo ""
    echo "Your dashboard is now live!"
    echo ""
    echo "Next steps:"
    echo "  1. Visit the deployment URL shown above"
    echo "  2. Add environment variables if this is first deployment:"
    echo "     vercel env add NEXT_PUBLIC_FIREBASE_API_KEY"
    echo "  3. Test with your ESP32 device"
    echo ""
else
    echo ""
    echo "❌ Deployment failed"
    echo "   Check the error messages above"
    echo ""
    exit 1
fi
