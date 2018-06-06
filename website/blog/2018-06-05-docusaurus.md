---
title: Trying Out Docusaurus
author: Hintron
authorURL: https://github.com/hintron
---

I was looking for a nice way to setup documentation, and [Docusaurus](https://docusaurus.io/) might be the answer!

 <!--truncate-->

## Evaluating Docusaurus

Things I want to try to figure out:
* Is there a way to automatically document my code using something like JavaDoc, so documentation is coupled with the code?\
* How does it work with gh-pages - I have my source in master, and then the website build on gh-pages?
* How do I modify the CSS?

## Commands

This is the command I use to build and publish to the gh-pages branch automatically:

    GIT_USER=hintron yarn run publish-gh-pages

To run an Live Reload local server:

    cd website
    yarn start


## Designing a logo

Of course, I wanted to replace the stupid dinosaur image that came with it. So I designed a 'halo' using [Figma](https://figma.com/).

Here's what I got:

### SVG
![celegraph_halo.svg](/celegraph/img/celegraph_halo.svg "The Celegraph Halo")

