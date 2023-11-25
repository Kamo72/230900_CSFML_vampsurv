#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <chrono>
#include <numbers>
#include <random>
#include <cmath>
#include <string>
#include <unordered_set>
#include <typeinfo>
#include <memory>
#include <any>
#include "SFML_Data.h"

using namespace std;
using namespace sf;

using namespace SFML_Data;


sf::Vector2f operator*(const sf::Vector2f& vector, float scalar)
{
    return sf::Vector2f(vector.x * scalar, vector.y * scalar);
}

template <typename T>
T dotProduct(const sf::Vector2<T>& vec1, const sf::Vector2<T>& vec2)
{
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

namespace SFML_Custom
{
    //��Ÿ ����
    
    float GetDirection(const Vector2f& point1, const Vector2f& point2) {
        float dx = point2.x - point1.x;
        float dy = point2.y - point1.y;

        // atan2 �Լ��� ����Ͽ� �� �� ������ ������ ����մϴ�.
        // ������� ���� �����̹Ƿ�, 0~360 ������ ��ȯ�ϱ� ����
        // ������ ���� ��ȯ�ϰ�, �������� ����� �����մϴ�.
        float direction = atan2(dy, dx) * 180 / 3.14159f;
        if (direction < 0) {
            direction += 360;
        }

        return direction;
    }

    Vector2f GetMoveDirection(float dir, float length) {
        // ������ �������� ��ȯ
        float radians = dir * (3.141592f / 180.0f);

        // x�� y ��ǥ�� �̵� �Ÿ� ���
        float dx = length * cos(radians);
        float dy = length * sin(radians);

        // sf::Vector2f�� ��ȯ
        return sf::Vector2f(dx, dy);
    }

    float GetDistance(sf::Vector2f p1, sf::Vector2f p2)
    {
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        float distance = sqrt(dx * dx + dy * dy);
        return distance;
    }

    random_device rd;   //random�Լ��� ���� ��������
    mt19937 generator(rd());   //random�Լ��� ���� ��������
    float random(float minVal, float maxVal) {
        float range = max(maxVal, minVal) - min(maxVal, minVal);
        uniform_real_distribution<float> distribution(0, max(maxVal, minVal) - min(maxVal, minVal));

        
        return distribution(generator) + min(maxVal, minVal);
    }
    int random(int minVal, int maxVal) {
        uniform_int_distribution<int> distribution(minVal, maxVal);
        return distribution(generator);
    }

    Vector2f GetBorderByDirection(Vector2f screenBorder, float direction) {
        // ȭ�� �߾� ��ǥ ���
        Vector2f screenCenter(screenBorder.x / 2.f, screenBorder.y / 2.f);

        // ���� ���� ���
        float radians = direction * (3.14159265359f / 180.f); // ������ �������� ��ȯ
        Vector2f directionVector(cos(radians), sin(radians));

        // �浹 ������ ��� ��ǥ ���
        Vector2f collisionPoint = screenCenter + directionVector * min(screenBorder.x, screenBorder.y) / 2.f;

        return collisionPoint;
    }
    
    bool isClockwiseClose(float dir1, float dir2) {
        // ���� ������ 0������ 360�� ���̷� ����ȭ�մϴ�.
        dir1 = fmod(dir1, 360.0f);
        dir2 = fmod(dir2, 360.0f);

        // �ð� ����� �ݽð� ������ ���̸� ����մϴ�.
        float clockwiseDiff = fmod(dir2 - dir1 + 360.0f, 360.0f);
        float counterclockwiseDiff = fmod(dir1 - dir2 + 360.0f, 360.0f);

        // �ð� ������ �� �����ٸ� true�� ��ȯ�ϰ�, �׷��� ������ false�� ��ȯ�մϴ�.
        return clockwiseDiff <= counterclockwiseDiff;
    }


    //Draw����
    bool LoadTexture(string imagePath, string textureName)
    {
        Texture* texture = new Texture();
        if (!(texture->loadFromFile(imagePath))) { return false;/*����*/ }
        textureDictionary.Add(textureName, texture);
        return true;
    }

    Sprite MakeSprite(string textureName)
    {
        Sprite sprite(*(textureDictionary.Get(textureName)));
        return sprite;
    }

    void DrawSprite(Sprite& sprite, float xPos, float yPos, float rotation = NULL,
        float width = NULL, float height = NULL, Color color = *drawColor, Uint8 alpha = drawAlpha)
    {
        if (width != NULL || height != NULL)
        {
            width = width == NULL ? sprite.getScale().x : width;
            height = height == NULL ? sprite.getScale().y : height;
        }
        float tRotation = rotation == NULL ? sprite.getRotation() : rotation;
        Color tempColor = { color.r,color.g,color.b, alpha };

        sprite.setPosition(xPos, yPos);
        sprite.setRotation(rotation);
        sprite.setScale(width, height);
        sprite.setColor(tempColor);

        (*nowWindow).draw(sprite);
    }

    void DrawRectangle(RectangleShape& rectangle, float xPos, float yPos, float rotation = NULL,
        float width = NULL, float height = NULL, Color color = *drawColor, Uint8 alpha = drawAlpha)
    {
        if (width != NULL || height != NULL)
        {
            width = width == NULL ? rectangle.getSize().x : width;
            height = height == NULL ? rectangle.getSize().y : height;
        }
        float tRotation = rotation == 0.0f ? rectangle.getRotation() : rotation;
        Color tempColor = { color.r, color.g, color.b, alpha };

        rectangle.setPosition(Vector2f(xPos, yPos));
        rectangle.setRotation(tRotation);
        rectangle.setSize(Vector2f(width, height));
        rectangle.setFillColor(tempColor);

        nowWindow->draw(rectangle);
    }

    void DrawCircle(CircleShape& circle, float xPos, float yPos, float rotation = NULL,
        float radius = 10.0f, Vector2f scale = Vector2f(1.0f, 1.0f), Color color = *drawColor, Uint8 alpha = drawAlpha)
    {
        circle.setPosition(Vector2f(xPos, yPos));
        float tRotation = rotation == 0.0f ? circle.getRotation() : rotation;
        Color tempColor = { color.r, color.g, color.b, alpha };

        
        if (scale == Vector2f(1.0f, 1.0f))
        {
            circle.setScale(scale);
        }
        
        nowWindow->draw(circle);
    }

    void DrawLine(Vector2f point1, Vector2f point2, float thickness = 1.0f, sf::Color color = sf::Color::White)
    {
        sf::ConvexShape lineShape(4);

        sf::Vector2f direction = point2 - point1;
        sf::Vector2f unitDirection = direction / sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

        sf::Vector2f offset = (thickness / 2.f) * unitPerpendicular;

        lineShape.setPoint(0, point1 + offset);
        lineShape.setPoint(1, point2 + offset);
        lineShape.setPoint(2, point2 - offset);
        lineShape.setPoint(3, point1 - offset);

        lineShape.setFillColor(color);

        (*nowWindow).draw(lineShape);
    }

    void DrawText(Text& text, float xPos, float yPos, Vector2f scale = Vector2f(0.0f,0.0f), Color color = Color::White, Font * font = nullptr)
    {
        text.setPosition(xPos, yPos);
        text.setFillColor(color);
        if (scale.x != 0) { text.setScale(text.getScale()); }
        text.setFont(*font);

        nowWindow->draw(text);
    }

    Vector2i screenSize = { 1920, 1080 };


    //Time ����
    long long GetCurrentTimeMillis()
    {
        auto current_time = chrono::system_clock::now();
        auto duration = current_time.time_since_epoch();
        return chrono::duration_cast<chrono::milliseconds>(duration).count();
    }

    long long startTime = GetCurrentTimeMillis();
    long long recentTime = startTime; //�ý��� ������ ����

    long long GetTime()
    {
        return GetCurrentTimeMillis() - startTime;
    }

    int frameRatio = 60;
    static float timeFromDraw = 0;
    static float timeToProcess = 0;

    long long deltaTime = 0;    //�����ӿ��� ����

    bool IsAbleToProcess() { return timeToProcess > 1000.0f / frameRatio; }
    bool IsAbleToDraw() { return timeFromDraw > 1000.0f / frameRatio; }
    void FrameTimeRefresh()
    {
        long long nowTime = GetTime();
        if (nowTime == recentTime) { return; }

        long long tempTime = nowTime - recentTime;
        if (abs(tempTime) > 50) {
            recentTime = GetTime();
            return;
        }
        timeFromDraw += static_cast<float>(tempTime);
        timeToProcess += static_cast<float>(tempTime);

        recentTime = GetTime();
    }


}

namespace Collision{

    class Line {
    public:
        sf::Vector2f start;
        sf::Vector2f end;
        Line() { start = end = { 0,0 }; };
        Line(const sf::Vector2f& startPos, const sf::Vector2f& endPos)
            : start(startPos), end(endPos) {}
    };

    class Point {
    public:
        sf::Vector2f position;

        Point(const sf::Vector2f& pos)
            : position(pos) {}
    };

    bool CheckCollision(const Line& line1, const Line& line2)
    {
        // Calculate the directional vectors of the two lines
        sf::Vector2f dir1 = line1.end - line1.start;
        sf::Vector2f dir2 = line2.end - line2.start;

        // Perform cross product calculations
        float cross1 = dir1.x * (line2.start.y - line1.start.y) - dir1.y * (line2.start.x - line1.start.x);
        float cross2 = dir1.x * (line2.end.y - line1.start.y) - dir1.y * (line2.end.x - line1.start.x);
        float cross3 = dir2.x * (line1.start.y - line2.start.y) - dir2.y * (line1.start.x - line2.start.x);
        float cross4 = dir2.x * (line1.end.y - line2.start.y) - dir2.y * (line1.end.x - line2.start.x);

        // Check for intersection between the two lines
        if ((cross1 * cross2 < 0) && (cross3 * cross4 < 0))
        {
            return true; // Collision detected
        }

        return false; // No collision
    }
    
    bool CheckCollision(const Line& line, const sf::RectangleShape& rectangle) {
        sf::FloatRect rectBounds = rectangle.getGlobalBounds();

        // Check if the line intersects with any of the four sides of the rectangle
        if (line.start.x > rectBounds.left && line.start.x < rectBounds.left + rectBounds.width &&
            line.start.y > rectBounds.top && line.start.y < rectBounds.top + rectBounds.height)
            return true;
        if (line.end.x > rectBounds.left && line.end.x < rectBounds.left + rectBounds.width &&
            line.end.y > rectBounds.top && line.end.y < rectBounds.top + rectBounds.height)
            return true;

        return false;
    }

    bool CheckCollision(const Line& line, const CircleShape& circle) {
        const sf::Vector2f& p1 = line.start;
        const sf::Vector2f& p2 = line.end;

        const Vector2f& center = circle.getPosition();
        const float radius = circle.getRadius();
        // Calculate the direction vector of the line segment
        sf::Vector2f lineDir = p2 - p1;

        // Calculate the vector from one endpoint of the line segment to the circle center
        sf::Vector2f circleToLine = center - p1;

        // Calculate the length of the line segment
        float lineLength = std::sqrt(lineDir.x * lineDir.x + lineDir.y * lineDir.y);

        // Calculate the dot product of the line direction vector and the vector to the circle center
        float dotProduct = (circleToLine.x * lineDir.x + circleToLine.y * lineDir.y) / (lineLength * lineLength);

        // Calculate the closest point on the line segment to the circle center
        sf::Vector2f closestPoint;
        if (dotProduct < 0.0f)
            closestPoint = p1;
        else if (dotProduct > 1.0f)
            closestPoint = p2;
        else
            closestPoint = p1 + dotProduct * lineDir;

        // Check if the distance between the closest point and the circle center is less than the circle radius
        sf::Vector2f closestPointToCenter = center - closestPoint;
        float distance = std::sqrt(closestPointToCenter.x * closestPointToCenter.x + closestPointToCenter.y * closestPointToCenter.y);
        return distance < radius;
    }

    bool CheckCollision(const Line& line, const Point& point) {
        // Check if the point is on the line segment
        float lineLengthSquared = (line.end.x - line.start.x) * (line.end.x - line.start.x) +
            (line.end.y - line.start.y) * (line.end.y - line.start.y);
        float dot1 = (point.position.x - line.start.x) * (line.end.x - line.start.x) +
            (point.position.y - line.start.y) * (line.end.y - line.start.y);
        float dot2 = (point.position.x - line.end.x) * (line.start.x - line.end.x) +
            (point.position.y - line.end.y) * (line.start.y - line.end.y);

        return dot1 >= 0 && dot2 >= 0 && dot1 <= lineLengthSquared && dot2 <= lineLengthSquared;
    }

    bool CheckCollision(const sf::RectangleShape& rectangle, const Line& line) {
        return CheckCollision(line, rectangle);
    }

    bool CheckCollision(const sf::RectangleShape& rectangle, const Point& point)
    {
        // Get the global bounds of the rectangle
        sf::FloatRect rectBounds = rectangle.getGlobalBounds();

        // Check if the point is inside the rectangle or on its edges
        if (rectBounds.contains(point.position) ||
            rectangle.getGlobalBounds().contains(point.position.x, point.position.y) ||
            rectangle.getGlobalBounds().contains(point.position.x, point.position.y + 1.f) ||
            rectangle.getGlobalBounds().contains(point.position.x + 1.f, point.position.y) ||
            rectangle.getGlobalBounds().contains(point.position.x + 1.f, point.position.y + 1.f))
        {
            return true; // Collision detected
        }

        return false; // No collision
    }

    bool CheckCollision(const sf::RectangleShape& rectangle1, const sf::RectangleShape& rectangle2) {
        sf::FloatRect rectBounds1 = rectangle1.getGlobalBounds();
        sf::FloatRect rectBounds2 = rectangle2.getGlobalBounds();

        return rectBounds1.intersects(rectBounds2);
    }

    bool CheckCollision(const sf::RectangleShape& rectangle, const sf::CircleShape& circle) {
        sf::Transform transform;
        transform.translate(rectangle.getPosition());
        transform.rotate(rectangle.getRotation());

        sf::FloatRect rectBounds = rectangle.getLocalBounds();
        sf::Vector2f rectCenter(rectBounds.width / 2.0f, rectBounds.height / 2.0f);
        rectCenter = transform.transformPoint(rectCenter);

        sf::Vector2f circleCenter = circle.getPosition();
        circleCenter = transform.getInverse().transformPoint(circleCenter);

        float circleRadius = circle.getRadius();

        sf::Vector2f closestPoint;
        closestPoint.x = std::max(rectBounds.left, std::min(circleCenter.x, rectBounds.left + rectBounds.width));
        closestPoint.y = std::max(rectBounds.top, std::min(circleCenter.y, rectBounds.top + rectBounds.height));

        sf::Vector2f distanceVec = circleCenter - closestPoint;
        float distanceSquared = distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y;

        return distanceSquared < (circleRadius* circleRadius);
    }

    bool CheckCollision(const sf::CircleShape & circle, const Line & line) {
        return CheckCollision(line, circle);
    }

    bool CheckCollision(const sf::CircleShape& circle, const Point& point) {
        sf::Vector2f circleCenter = circle.getPosition();
        float circleRadius = circle.getRadius();

        // Calculate the distance between the circle center and the point
        float distanceSquared = (circleCenter.x - point.position.x) * (circleCenter.x - point.position.x) +
            (circleCenter.y - point.position.y) * (circleCenter.y - point.position.y);

        // Check if the distance is less than the circle radius
        return distanceSquared <= (circleRadius * circleRadius);
    }

    bool CheckCollision(const sf::CircleShape& circle, const sf::RectangleShape& rectangle)
    {
        return CheckCollision(rectangle, circle);
    }

    bool CheckCollision(const sf::CircleShape& circle1, const sf::CircleShape& circle2) {
        sf::Vector2f center1 = circle1.getPosition();
        sf::Vector2f center2 = circle2.getPosition();
        float radius1 = circle1.getRadius();
        float radius2 = circle2.getRadius();

        // Calculate the distance between the centers of the circles
        float distanceSquared = (center1.x - center2.x) * (center1.x - center2.x) +
            (center1.y - center2.y) * (center1.y - center2.y);

        // Check if the distance is less than the sum of the radii
        return distanceSquared <= ((radius1 + radius2) * (radius1 + radius2));
    }

    bool CheckCollision(const Point& point, const Line& line) {
        return CheckCollision(line, point);
    }

    bool CheckCollision(const Point& point, const sf::RectangleShape& rectangle) {
        return CheckCollision(rectangle, point);
    }

    bool CheckCollision(const Point& point, const sf::CircleShape& circle) {
        return CheckCollision(circle, point);
    }
}
