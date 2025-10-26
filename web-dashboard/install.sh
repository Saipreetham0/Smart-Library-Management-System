#!/bin/bash

# Smart Library Dashboard - Installation Script
# This script will set up the dashboard for you

echo "================================================"
echo "  Smart Library Dashboard - Installation"
echo "================================================"
echo ""

# Check if Node.js is installed
if ! command -v node &> /dev/null; then
    echo "❌ Node.js is not installed"
    echo "   Please install Node.js from https://nodejs.org/"
    echo "   Version 18 or higher required"
    exit 1
fi

echo "✅ Node.js version: $(node --version)"

# Check if pnpm is installed
if ! command -v pnpm &> /dev/null; then
    echo "⚠️  pnpm is not installed"
    echo "   Installing pnpm globally..."
    npm install -g pnpm
    if [ $? -ne 0 ]; then
        echo "❌ Failed to install pnpm"
        exit 1
    fi
    echo "✅ pnpm installed successfully"
else
    echo "✅ pnpm version: $(pnpm --version)"
fi

echo ""
echo "Installing dependencies..."
echo ""

# Install dependencies
pnpm install

if [ $? -ne 0 ]; then
    echo "❌ Installation failed"
    exit 1
fi

echo ""
echo "================================================"
echo "  Installation Complete! 🎉"
echo "================================================"
echo ""

# Check if .env.local exists
if [ ! -f .env.local ]; then
    echo "⚠️  Configuration file .env.local not found"
    echo ""
    echo "Creating .env.local from .env.example..."
    cp .env.example .env.local
    echo "✅ .env.local created"
    echo ""
    echo "⚠️  IMPORTANT: Please update .env.local with your Firebase credentials"
    echo ""
fi

echo "Next steps:"
echo ""
echo "  1. Update .env.local with your Firebase credentials (if needed)"
echo "  2. Run the dashboard:"
echo ""
echo "     pnpm dev"
echo ""
echo "  3. Open http://localhost:3000 in your browser"
echo ""
echo "================================================"
echo ""
