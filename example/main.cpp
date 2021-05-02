// This file is part of zorder_knn.
//
// Copyright(c) 2010, 2021 Sebastian Lipponer
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#include <array>

#include <sstream>
#include <iomanip>
#include <fstream>

#include <random>
#include <vector>
#include <algorithm>
#include <zorder_knn/less.hpp>

#include <cstdlib>

template <typename T, std::size_t d> using Pt = std::array<T, d>;
using Pt2 = Pt<float, 2>;

std::string
svg_circle(std::size_t id, Pt2 const& c_xy)
{
    std::stringstream circle;
    circle << "<circle id=\"circle" << id << "\" cx=\"" << c_xy[0] << "\" cy=\"" << c_xy[1] << "\" r=\"0.5\"/>";
    return circle.str();
}

std::string
svg_path(std::size_t id, float s, Pt2 const& p0, Pt2 const& p1)
{
    std::stringstream path;
    path << "<path id=\"path" << id << "\" style=\"stroke:#"
         << std::setfill('0') << std::hex
         << std::setw(2) << static_cast<int>(255.0f * s)
         << std::setw(2) << 0u
         << std::setw(2) << static_cast<int>(255.0f * (1.0 - s))        
         << ";stroke-width:0.4\" d=\"M "
         << p0[0] << "," << p0[1] << " " << p1[0] << "," << p1[1] << "\"/>";
    return path.str();
}

void
svg_save(std::string filename, std::vector<Pt2> const& pts)
{
    const std::string header =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
        "<svg id=\"svg8\""
            " xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\""
            " xmlns=\"http://www.w3.org/2000/svg\""
            " width=\"400px\" height=\"400px\""
            " version=\"1.1\""
            " xmlns:cc=\"http://creativecommons.org/ns#\""
            " xmlns:dc=\"http://purl.org/dc/elements/1.1/\""
            " viewBox=\"-55 -55 110 110\">\n"
        "  <metadata id=\"metadata5\">\n"
        "    <rdf:RDF>\n"
        "      <cc:Work rdf:about=\"\">\n"
        "        <dc:format>image/svg+xml</dc:format>\n"
        "        <dc:type"
            " rdf:resource=\"http://purl.org/dc/dcmitype/StillImage\" />\n"
        "        <dc:title></dc:title>\n"
        "      </cc:Work>\n"
        "    </rdf:RDF>\n"
        "  </metadata>\n"
        "<g id=\"layer1\">\n";

    const std::string footer =
        "</g>\n"
        "</svg>\n";

    std::ofstream svg(filename);
    svg << header;
    for (std::size_t i(0); i < pts.size() - 1; ++i)
    {
        svg << "  "
            << svg_path(i, static_cast<float>(i) / (pts.size() - 2),
                   pts[i], pts[i + 1])
            << "\n";
    }
    for (std::size_t i(0); i < pts.size(); ++i)
    {
        svg << "  "
            << svg_circle(i, pts[i])
            << "\n";
    }
    svg << footer;
    svg.close();
}

int
main(int argc, char* argv[])
{
    std::random_device rd;
    std::mt19937 e2(rd());

    std::vector<Pt2> pts(2000);
    std::uniform_real_distribution<float> dist(-50.0, 50.0);
    std::generate(pts.begin(), pts.end(), [&] {
        Pt2 p;
        std::generate(p.begin(), p.end(), [&] { return dist(e2); });
        return p;
    });

    svg_save("example_random.svg", pts);
    std::sort(pts.begin(), pts.end(), zorder_knn::less<Pt2, 2>());
    svg_save("example_morton.svg", pts);

    return EXIT_SUCCESS;
}
