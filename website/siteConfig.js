/**
 * Copyright (c) 2017-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// See https://docusaurus.io/docs/site-config.html for all the possible
// site configuration options.

/* List of projects/orgs using your project for the users page */
// const users = [
//   {
//     caption: 'User1',
//     // You will need to prepend the image path with your baseUrl
//     // if it is not '/', like: '/test-site/img/docusaurus.svg'.
//     image: '/img/docusaurus.svg',
//     infoLink: 'https://www.facebook.com',
//     pinned: true,
//   },
// ];

const siteConfig = {
  title: 'Celegraph',
  tagline: 'An Agelically Fast UDP Messaging Framework',
  // url: 'https://your-docusaurus-test-site.com' /* your website url */,
  // baseUrl: '/' /* base url for your project */,
  // For github.io type URLs, you would set the url and baseUrl like:
  url: 'https://hintron.github.io',
  baseUrl: '/celegraph/',

  // Used for publishing and more
  projectName: 'celegraph',
  organizationName: 'hintron',
  // For top-level user or org sites, the organization is still the same.
  // e.g., for the https://JoelMarcey.github.io site, it would be set like...
  //   organizationName: 'JoelMarcey'

  // For no header links in the top nav bar -> headerLinks: [],
  headerLinks: [
    {doc: 'usage', label: 'Docs'},
    {blog: true, label: 'Blog'},
    {href: 'https://github.com/hintron/celegraph', label: 'GitHub'},
    // {page: 'help', label: 'Help'},
  ],

  blogSidebarCount: 'ALL',

  // If you have users set above, you add it here:
  // users,

  /* path to images for header/footer */
  headerIcon: 'img/celegraph_halo.svg',
  footerIcon: 'img/celegraph_halo.svg',
  favicon: 'img/favicon.png',

  /* colors for website */
  colors: {
    primaryColor: '#fcd32d',
    secondaryColor: '#ffe168',
  },

  /* custom fonts for website */
  // fonts: {
  //   myFont: [
  //     "Consolas",
  //     "Sans Serif"
  //   ],
  //   // myOtherFont: [
  //   //   "-apple-system",
  //   //   "system-ui"
  //   // ]
  // },

  // This copyright info is used in /core/Footer.js and blog rss/atom feeds.
  copyright:
    'Copyright © ' +
    new Date().getFullYear() +
    ' Michael Hinton',

  highlight: {
    // Highlight.js theme to use for syntax highlighting in code blocks
    theme: 'default',
  },

  // Add custom scripts here that would be placed in <script> tags
  scripts: ['https://buttons.github.io/buttons.js'],

  /* On page navigation for the current documentation page */
  onPageNav: 'separate',

  /* Open Graph and Twitter card images */
  ogImage: 'img/celegraph_halo_filled.png',
  twitterImage: 'img/celegraph_halo_filled.png',

  // You may provide arbitrary config keys to be used as needed by your
  // template. For example, if you need your repo's URL...
  repoUrl: 'https://github.com/hintron/celegraph',
};

module.exports = siteConfig;
